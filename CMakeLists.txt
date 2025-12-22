cmake_minimum_required(VERSION 4.0)
project(letkv_cli)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE src ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(letkv-cli ${src})

target_include_directories(letkv-cli 
        PUBLIC ${CMAKE_SOURCE_DIR}/inc/
)