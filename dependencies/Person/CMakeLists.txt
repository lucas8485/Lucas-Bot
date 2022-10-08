cmake_minimum_required(VERSION 3.24)
project(Person)

set(CMAKE_CXX_STANDARD 17)

add_library(Person SHARED Person.cpp PersonCollection.cpp)
