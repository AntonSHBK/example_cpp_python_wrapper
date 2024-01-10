cmake_minimum_required(VERSION 3.26.5 FATAL_ERROR)

project(
  cpp_to_python 
  VERSION 0.1.0
  DESCRIPTION "cpp_to_python description"
)

# Standarts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(
    SOURCES 
    python/python_header.h
    python/python_src.cpp
)

add_executable(
    ${PROJECT_NAME} 
    ${SOURCES} 
)

if (NOT TARGET cpp_code)
  add_subdirectory(cpp)
endif() 
target_link_libraries(${PROJECT_NAME} cpp_code)



