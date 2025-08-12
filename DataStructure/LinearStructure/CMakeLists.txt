cmake_minimum_required(VERSION 3.10)

project(Linear VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ArrayList ArrayList.cpp ArrayList.h linear_list.h)