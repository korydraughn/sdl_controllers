cmake_minimum_required(VERSION 3.10)
project(controller_test LANGUAGES C CXX)

find_package(SDL2 REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(TARGET controller_test)

add_executable(${TARGET} main.cpp)
target_compile_options(${TARGET} PRIVATE -std=c++17 -Wall -Wextra)
target_include_directories(${TARGET} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${TARGET} PRIVATE  ${SDL2_LIBRARIES})
