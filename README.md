<p align="right">
  <img width="25%" height="25%"src="./images/klepsydra_logo.jpg">
</p>

# Installation Instructions

## System dependencies

* Ubuntu 14.04 or above
* ConcurrentQueue (https://github.com/klepsydra-technologies/concurrentqueue)
* Cereal (https://github.com/klepsydra-technologies/cereal)
* ROS Indigo or above (optional)
* DDS (optional)
* Cmake 3.5.1 or above
* gcc for C++11 5.4.0 or above.
* Doxygen (optional)
* Moxygen (https://github.com/sourcey/moxygen) (optional)
* Open CV
* Ros Vision Package

## Klepsydra dependencies

* kpsr-serialization
* kpsr-core

## System installation

	sudo apt install build-essentials
	sudo apt install git
	sudo apt install cmake
	git clone https://github.com/google/googletest.git

### Google Test

Google-Tests has to be at the level of the part of the folder where Klepsydra core will be installed. For example, if Klepsydra is installed in:
	$KLEPSYDRA_HOME/kpsr-core

Then google tests has to be installed in:
	$KLEPSYDRA_HOME/../googletest

These locations can be overridden by including the variables `GTEST_PATH` and `THIRDPARTIES_PATH` in the kpsr-core cmake invocation.

### Cereal
	git clone https://github.com/klepsydra-technologies/cereal
	sudo mkdir $THIRDPARTIES_PATH/include
	sudo cp cereal/include/* $THIRDPARTIES_PATH/include

By default, we choose the location of installation for Cereal to be /opt/klepsydra/thirdparties.
This location can be overridden by including the variable

	THIRDPARTIES_PATH

### Concurrent queue

	git clone https://github.com/klepsydra-technologies/concurrentqueue
	sudo mkdir $THIRDPARTIES_PATH/include
	sudo cp concurrentqueue/*.h $THIRDPARTIES_PATH/include

By default, we choose the location of installation for ConcurrentQueue to be /opt/klepsydra/thirdparties.
This location can be overridden by including the variable

	THIRDPARTIES_PATH

## Installation

Given ```$KLEPSYDRA_HOME```, for example ```$HOME/klepsydra```:

```
git clone https://github.com/kpsr-devs/kpsr-vision-ocv.git
cd kpsr-vision-ocv
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
* -DKPSR_WITH_DDS=true for building the DDS binding
* -DKPSR_INSTALL_PATH for specifying the Klepsydra installation location (/opt/klepsydra by default)
* -DGTEST_PATH for the google test path (default is ../../googletest)
* -DTHIRDPARTIES_PATH for the ConcurrentQueue and Cereal path (default is /opt/klepsydra/thirdparties)
* -DKPSR_BUILD_PATH location of the ```kpsr-build``` repository
* -DKPSR_WITH_DOXYGEN to allow generation of documentation

### ROS installation

In case of a new project:

```
mkdir -p YOUR_ROS_PROJECT
cd YOUR_ROS_PROJECT
source /opt/ros/melodic/setup.bash
catkin_init_workspace
```

Then add the Klepsydra ROS sensors project to the ROS project:

```
cd YOUR_ROS_PROJECT
ln -s ../../robotics_utils/kpsr-vision-ocv/modules/ros_mdlw/kpsr_ros_vision_ocv
```

# Documentation

## Documentation generation

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
