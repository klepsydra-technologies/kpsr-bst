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
* BST SDK
* Doxygen (optional)
* Moxygen (https://github.com/sourcey/moxygen) (optional)

## Klepsydra dependencies

* kpsr-core
* kpsr-robotics
* kpsr-admin (optional)

## Installation

Given ```$KLEPSYDRA_HOME```, for example ```$HOME/klepsydra```:

```
git clone https://github.com/kpsr-devs/kpsr-vision-ocv.git
cd kpsr-bst
mkdir build
cd build
cmake ..
make
make test
sudo make install
```

This will install the klespydra vision-ocv in

	/opt/klepsydra	

The cmake has the following options:
* -DKPSR_WITH_ADMIN=true for real-time performance and trajectory monitoring tool
* -DKPSR_WITH_DDS=true for building the DDS binding
* -DKPSR_WITH_ZMQ=true for building the ZMQ binding
* -DKPSR_INSTALL_PATH for specifying the Klepsydra installation location (/opt/klepsydra by default)
* -DBST_SDK_HOME for specifying the location of the SDK path.
* -DKPSR_WITH_DOXYGEN to allow generation of documentation

Example:


```
cmake -DKPSR_WITH_ZMQ=true -DBST_SDK_HOME=/home/user1/development/swiftpilot/SDK ..
```


## Documentation

### Documentation generation

```
make doc
```

### Location of documentation

The last built documentation is available in [Klepsydra Robotics API DOC](./api-doc/)


#  License

&copy; Copyright 2019-2020, Klepsydra Technologies, all rights reserved. Licensed under the terms in [LICENSE.md](./LICENSE.md)

This software and documentation are Copyright 2019-2020, Klepsydra Technologies
Limited and its licensees. All rights reserved. See [license file](./LICENSE.md) for full copyright notice and license terms.

#  Contact

https://www.klepsydra.com
support@klepsydra.com

