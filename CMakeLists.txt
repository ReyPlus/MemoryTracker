cmake_minimum_required(VERSION 3.28.1)

project(MemoryTracker)

add_executable(${PROJECT_NAME} src/main.cpp)

# Subdirectories
add_subdirectory(src)