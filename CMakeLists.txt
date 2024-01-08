cmake_minimum_required(VERSION 3.27)
project(mros_json)

set(CMAKE_CXX_STANDARD 20)

add_executable(mros_json main.cpp
        mros_json.cpp)
