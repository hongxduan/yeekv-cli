cmake_minimum_required(VERSION 4.0)
project(yeekv_cli)

set(CMAKE_CXX_STANDARD 23)

add_executable(yeekv-cli main.cpp
        include/argparse.hpp
        kvtp/request.cpp
        kvtp/request.h
        kvtp/response.cpp
        kvtp/response.h
        parser/input_parser.cpp
        parser/input_parser.h
        util/string_util.cpp
        util/string_util.h
        util/byte_util.cpp
        util/byte_util.h)
