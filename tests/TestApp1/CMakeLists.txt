cmake_minimum_required(VERSION 3.3)
project (TestApp1)
 
set (INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set (SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
 
set(APP_SRCS
	${SRC_DIR}/main.cpp
)
 
add_executable(TestApp1 ${APP_SRCS})
include_directories("${FUNCPP_MODULES_DIR}/typeclass/include")

