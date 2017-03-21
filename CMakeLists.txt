cmake_minimum_required(VERSION 3.3)
project (rootproject)


# set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++1z -g -Wno-c++1z-extensions") # don't warn about C++1z extensions

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}) 
set(FUNCPP_MODULES_DIR ${CMAKE_SOURCE_DIR}/modules)
set(FUNCPP_EXTERNALS_DIR ${CMAKE_SOURCE_DIR}/externals)

add_subdirectory(${FUNCPP_MODULES_DIR}/basetypes)
add_subdirectory(${FUNCPP_MODULES_DIR}/typeclass)
add_subdirectory(${FUNCPP_MODULES_DIR}/io)
add_subdirectory(${FUNCPP_MODULES_DIR}/testsupport)
add_subdirectory(${FUNCPP_MODULES_DIR}/mpl)
add_subdirectory(${FUNCPP_MODULES_DIR}/playground)

