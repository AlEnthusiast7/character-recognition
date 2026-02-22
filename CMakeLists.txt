cmake_minimum_required(VERSION 3.10)
project(CharacterRecognition)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp
src/Grid.cpp)

find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
  PRIVATE SDL3
  GL)
