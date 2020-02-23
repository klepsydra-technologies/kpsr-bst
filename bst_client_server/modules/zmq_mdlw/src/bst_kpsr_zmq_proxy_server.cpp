/*****************************************************************************
*
*                           Klepsydra Core Modules
*              Copyright (C) 2019-2020  Klepsydra Technologies GmbH
*                            All Rights Reserved.
*
*  This file is subject to the terms and conditions defined in
*  file 'LICENSE.md', which is part of this source code package.
*
*  NOTICE:  All information contained herein is, and remains the property of Klepsydra
*  Technologies GmbH and its suppliers, if any. The intellectual and technical concepts
*  contained herein are proprietary to Klepsydra Technologies GmbH and its suppliers and
*  may be covered by Swiss and Foreign Patents, patents in process, and are protected by
*  trade secret or copyright law. Dissemination of this information or reproduction of
*  this material is strictly forbidden unless prior written permission is obtained from
*  Klepsydra Technologies GmbH.
*
*****************************************************************************/
/* STD LIBS */
#include <string>
#include <getopt.h>
#include <time.h>
#include <unistd.h>

/* ZMQ LIBS */
#include <czmq.h>

/* KPSR LIBS */
#include <klepsydra/core/yaml_environment.h>

#include <klepsydra/zmq_core/zhelpers.hpp>

#include <klepsydra/bst_client_server/bst_server_user_input.h>
#include <klepsydra/bst_client_server/bst_main_helper.h>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

class ZmqProxy {
public:
    ZmqProxy(std::string frontendUrl, std::string backendUrl, bool bindFrontend)
        : _running(false)
        , _context(1)
        , _frontend(_context, ZMQ_SUB)
        , _backend(_context, ZMQ_XPUB)
    {
        //  This is where the weather server sits
        if (bindFrontend) {
            _frontend.bind(frontendUrl);
        } else {
            _frontend.connect(frontendUrl);
        }

        //  This is our public endpoint for subscribers
        _backend.bind(backendUrl);
        _backend.setsockopt(ZMQ_XPUB_VERBOSE, 1);

        //  Subscribe on everything
        _frontend.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    }

    void start () {
        _running = true;
        _thread = std::thread([this]() {
            while (_running) {
                run();
            }
        });
    }

    void stop() {
        _running = false;
        if (_thread.joinable()) {
            _thread.join();
        }
    }

protected:
    virtual void run() = 0;

    std::atomic<bool> _running;
    std::thread _thread;
    zmq::context_t _context;
    zmq::socket_t _frontend;
    zmq::socket_t _backend;
};

class ZmqBasicProxy : public ZmqProxy {
public:
    ZmqBasicProxy(std::string frontendUrl, std::string backendUrl)
        : ZmqProxy(frontendUrl, backendUrl, false)
    {}

protected:
    void run() override {
        while (1) {
            zmq::message_t message;
            int more;
            size_t more_size = sizeof (more);

            //  Process all parts of the message
            _frontend.recv(&message);
            _frontend.getsockopt( ZMQ_RCVMORE, &more, &more_size);
            _backend.send(message, more? ZMQ_SNDMORE: 0);
            if (!more)
                break;      //  Last message part
        }
    }
};

class ZmqLvcProxy : public ZmqProxy {
public:
    ZmqLvcProxy(std::string frontendUrl, std::string backendUrl, int timeout)
        : ZmqProxy(frontendUrl, backendUrl, true)
        , _timeout(timeout)
    {}

protected:
    void run() override {
        zmq::pollitem_t items [] = {
            { _frontend, 0, ZMQ_POLLIN, 0 },
            { _backend,  0, ZMQ_POLLIN, 0 }
        };
        if (zmq::poll(items, 2, _timeout) == -1)
            return;              //  Interrupted

        if (items[0].revents & ZMQ_POLLIN) {
            std::string topic = s_recv (_frontend);
            std::string contents = s_recv (_frontend);
            spdlog::info("{}new message received from server. Topic {}. Contents: {}", __PRETTY_FUNCTION__, topic, contents);
            _cache[topic] = contents;
            s_sendmore (_backend, topic);
            s_send (_backend, contents.c_str());
        }

        if (items [1].revents & ZMQ_POLLIN) {
            spdlog::info("{}new subcriber connected for topic.0", __PRETTY_FUNCTION__);
            zframe_t *frame = zframe_recv (_backend);
            if (!frame)
                return;
            //  Event is one byte 0=unsub or 1=sub, followed by topic
            byte *event = zframe_data (frame);
            spdlog::info("{}new subcriber connected for topic: {}", __PRETTY_FUNCTION__, event [0]);
            if (event [0] == 1) {
                std::string topic(event + 1, event + zframe_size (frame));
                std::map<std::string, std::string>::iterator it;
                it = _cache.find(topic);
                if (it != _cache.end()) {
                    s_sendmore (_backend, topic);
                    s_send (_backend, it->second);
                    spdlog::info("{}new subcriber connected for topic {}. Sending contents: {}", __PRETTY_FUNCTION__, topic, it->second);
                }
            }
            zframe_destroy (&frame);
        }
    }

private:
    std::chrono::milliseconds _timeout;
    std::map<std::string, std::string> _cache;
};

int main(int argc, char *argv[])
{
    std::string fileName = kpsr::bst::BstMainHelper::getConfFileFromParams(argc, argv);

    kpsr::YamlEnvironment yamlEnv(fileName);

    std::string serverPublishUrl; // frontend
    std::string serverSubscribeUrl; // backend
    yamlEnv.getPropertyString("bst_server_publish_url", serverPublishUrl);
    yamlEnv.getPropertyString("bst_server_subscribe_url", serverSubscribeUrl);
    ZmqBasicProxy serverProxy(serverPublishUrl, serverSubscribeUrl);

    std::string clientPublishUrl; // frontend
    std::string clientSubscribeUrl; // backend
    yamlEnv.getPropertyString("bst_client_publish_url", clientPublishUrl);
    yamlEnv.getPropertyString("bst_client_subscribe_url", clientSubscribeUrl);
    ZmqBasicProxy clientProxy(clientPublishUrl, clientSubscribeUrl);

    std::string envWriteUrl; // frontend
    std::string envListenUrl; // backend
    int timeout;
    yamlEnv.getPropertyString("kpsr_zmq_env_write_url", envWriteUrl);
    yamlEnv.getPropertyString("kpsr_zmq_env_listen_url", envListenUrl);
    yamlEnv.getPropertyInt("kpsr_zmq_env_timeout", timeout);
    ZmqLvcProxy environmentProxy(envWriteUrl, envListenUrl, timeout);

    BstServerUserInput bstServerUserInput;

    serverProxy.start();
    clientProxy.start();
    environmentProxy.start();

    bstServerUserInput.run();

    environmentProxy.stop();
    clientProxy.stop();
    serverProxy.stop();

    printf("Disconnected, exiting.\n\n");
}
