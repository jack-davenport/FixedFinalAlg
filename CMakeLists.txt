cmake_minimum_required(VERSION 3.15)

project(FixedFinalAlg)
set(CMAKE_CXX_STANDARD 14)
set (input01 "ctrl.txt")
set (input02 "Pride.txt")
set (input03 "sample.txt")
set (input04 "shakespeare.txt")

foreach(file IN LISTS input01 input02 input03 input04)
        configure_file(${file} ${file} COPYONLY)
endforeach()
add_executable(FixedFinalAlg main.cpp)