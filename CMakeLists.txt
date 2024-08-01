cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cacs305-framework)

add_subdirectory(src)

option(CACS305_BUILD_EXAMPLES OFF)
if(CACS305_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()