cmake_minimum_required(VERSION 3.3)
project (TestApp1)
 
set (PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set (PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
 
set(APP_SRCS
	${PROJECT_SOURCE_DIR}/main.cpp
)
 
include_directories("${PROJECT_INCLUDE_DIR}")

add_executable(TestApp1 ${APP_SRCS})
