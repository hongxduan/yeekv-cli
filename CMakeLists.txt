cmake_minimum_required(VERSION 4.0)
project(yeekv_cli)

set(CMAKE_CXX_STANDARD 20)

add_executable(yeekv-cli main.cpp
        include/argparse.hpp
        kvtp/request.cpp
        kvtp/request.h
        kvtp/response.cpp
        kvtp/response.h
        parser/input_parser.cpp
        parser/input_parser.h
        utility/string_utility.cpp
        utility/string_utility.h)
