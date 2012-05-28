cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE MAIN_SRC "src/*.h" "src/*.c" "src/*.cpp")

find_package(SDL REQUIRED)

add_executable(main ${MAIN_SRC})
target_link_libraries(main SDL GL GLU)
add_definitions(`sdl-config --cflags`)

include_directories("libs/include")
include_directories("src")


set(CMAKE_CXX_FLAGS "-std=c++0x -g -Wall")