cmake_minimum_required(VERSION 3.14)

project("Destiny")

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/Engine/*.cpp")

add_executable(Destiny ${SOURCES})
