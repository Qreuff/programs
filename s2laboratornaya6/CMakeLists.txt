cmake_minimum_required(VERSION 3.10)
project(StudentProject)

option(BUILD_SHARED_LIBS "Build shared libraries instead of static" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)   
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  

add_library(student_lib ${CMAKE_CURRENT_SOURCE_DIR}/student.c)
target_include_directories(student_lib PUBLIC include)

add_executable(student_app main.c)
target_link_libraries(student_app student_lib)