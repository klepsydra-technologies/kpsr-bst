<p align="right">
  <img width="25%" height="25%"src="./images/klepsydra_logo.jpg">
</p>

# Installation Instructions

## System dependencies

* Ubuntu 14.04 or above
* ZMQ (optional)
* DDS (optional)
* Cmake 3.5.1 or above
* gcc for C++11 5.4.0 or above.
* BST SDK (https://gitlab.com/bstaero/sdk)
* Doxygen (optional)
* Moxygen (https://github.com/sourcey/moxygen) (optional)

## Klepsydra dependencies

* kpsr-core
* kpsr-robotics
* kpsr-admin (optional)

## Installation

Given ```$KLEPSYDRA_HOME```, for example ```$HOME/klepsydra```:

```
git clone https://bitbucket.org/kpsr-devs/kpsr-bst.git
cd kpsr-bst
mkdir build
cd build
cmake ..
make
make test
sudo make install
```

This will install the klespydra bst in

	/opt/klepsydra

The cmake has the following options:
* -DKPSR_WITH_DDS=true for building the DDS binding
* -DKPSR_WITH_ZMQ=true for building the ZeroMQ binding.
* -DBST_SDK_HOME. Location of BST sdk repo
* -DCMAKE_PREFIX_PATH for the Pistache install path (pistache/build)
Example:


```
cmake -DKPSR_WITH_ZMQ=true -DBST_SDK_HOME=/home/pablogh/development/swiftpilot/Autopilot/sdk -DCMAKE_PREFIX_PATH=/home/pablogh/development/pistache/build ..
```

### Installation packages.

Besides the API libraries for the use of KPSR-BST, several executables are included in this repo:


In memory Eventloop client server. Example of use:
```
./bin/bst_mem_el_client_server -f ../stages/memstg/bst_mem_el_client_server/conf/bst_mem_el_client_server.yaml

ZMQ Executables: BST server, client and proxy:
```
./bin/bst_zmq_server -f ../stages/zmqstg/bst_zmq_client_server/conf/bst_zmq_server.yaml
./bin/bst_zmq_client -f ../stages/zmqstg/bst_zmq_client_server/conf/bst_zmq_client.yaml
./bin/bst_kpsr_zmq_proxy_server -f ../stages/zmqstg/bst_zmq_client_server/conf/bst_zmq_proxy_server.yaml
```

## Documentation

### Documentation generation

```
make doc
```

### Location of documentation

The last built documentation is available in [Klepsydra BST API DOC](./api-doc/)


#  License

&copy; Copyright 2019-2020, Klepsydra Technologies, all rights reserved. Licensed under the terms in [LICENSE.md](./LICENSE.md)

This software and documentation are Copyright 2019-2020, Klepsydra Technologies
Limited and its licensees. All rights reserved. See [license file](./LICENSE.md) for full copyright notice and license terms.

#  Contact

https://www.klepsydra.com
support@klepsydra.com

