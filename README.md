# Python wrapper for Emergent Cameras being used at NYU CUSP


## Install instructions:
Linux (Ubuntu):
- **Install BOOST python lib: **`sudo apt-get install libboost-all-dev`
- **Install CMAKE: **`sudo apt-get install cmake`

MAC:
- **Install BOOST python lib: ** `brew install boost`

WINDOWS:
- *Refer `www.boost.org/doc/libs/1_58_0/more/getting_started/windows.html` *


## Instructions for creating CMakeLists file:
**Example CMakeLists.txt**

```
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "DEBUG")
  ENDIF()

FIND_PACKAGE(Boost 1.55)
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.7")
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost 1.55.0 COMPONENTS python)

  ADD_LIBRARY(<shared file object name to be created> SHARED <cpp filename>)
  TARGET_LINK_LIBRARIES(hello_ext ${Boost_LIBRARIES})
ELSEIF(NOT Boost_FOUND)
      MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS("-Wall")
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
  ENDIF()
  
```
> I still need to understand details about cmake file but this will do for now..

## Developer Notes:
*Instructions for compiling and creating shared object*

- **Create cmake (`CMakeLists.txt`) file in the same directory as the cxx code **
- **create a directory `build` and enter it **
- **run `cmake ..` **
- **run `make` **
- Open ipython and import the module

*Copyright 2015 Mohit Sharma*