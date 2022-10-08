cmake_minimum_required(VERSION 3.24)
project(Http)

set(CMAKE_CXX_STANDARD 17)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(Http SHARED library.cpp)
target_link_libraries(Http ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcurl.dll.a)