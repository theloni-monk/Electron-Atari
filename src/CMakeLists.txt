cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Electron-Atari)
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)
add_executable(app main.cpp CPU.cpp Operations.cpp TestEnv.cpp)