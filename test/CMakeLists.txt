CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE "DEBUG")
ENDIF()

FIND_PACKAGE(Boost)
IF(Boost_FOUND)
  # For some reason boost libraries cannot be found!
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.7")
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)

  FIND_PACKAGE(PythonInterp)
  FIND_PACKAGE(PythonLibs)
  FIND_PACKAGE(Boost COMPONENTS python)
  
  # This is how it should be if the boost libraries are found in env
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${Python_INCLUDE_DIRS})
  LINK_LIBRARIES(${Boost_LIBRARIES} ${Python_LIBRARIES})
  # Let boost handle from here
  PYTHON_ADD_MODULE(hello_ext hello_ext.cpp)
  
ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
ENDIF()

IF(CMAKE_COMPILER_IS_GNUCXX)
  ADD_DEFINITIONS("-Wall")
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
  ENDIF()
