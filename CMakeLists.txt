cmake_minimum_required(VERSION 3.10)
project(CR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp
src/Grid.cpp
src/Math/Matrix.cpp
src/MnistLoader.cpp
src/MnistNetwork.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(SDL3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
  PRIVATE SDL3
  )
