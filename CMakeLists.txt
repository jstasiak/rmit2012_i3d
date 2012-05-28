cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE MAIN_SRC "src/*.h" "src/*.c" "src/*.cpp")

find_package(SDL REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_image REQUIRED)

add_executable(main ${MAIN_SRC})
target_link_libraries(main SDL GL GLU SDL_ttf SDL_image)
add_definitions(`sdl-config --cflags`)

include_directories("libs/include")
include_directories("src")


set(CMAKE_CXX_FLAGS "-std=c++0x -g -Wall -fpermissive -lboost_program_options")
