CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#ds project name
PROJECT(picp)

#ds set cmake compilation flags
SET(CMAKE_CXX_FLAGS "-Wall -std=c++11") #ds enable all warnings and c++11 features
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG") #ds enable maximum optimization and disable debug symbols for cpp
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3 -DNDEBUG") #ds enable maximum optimization and disable debug symbols for c
SET(CMAKE_BUILD_TYPE Release)

#ds find and set up dependencies
FIND_PACKAGE(Eigen3 REQUIRED)
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(src)

#ds add build targets
add_subdirectory(executables)
