cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(
  CppToPython 
  VERSION 0.1.0
  DESCRIPTION "CppToPython description"
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
  PYTHON_SOURCES 
  python/python_header.h
  python/python_src.cpp
)

file (
  GLOB
  CPP_SOURCES 
  "cpp/src/*.cpp"
  "cpp/include/my_lib/*.h"
)


if (NOT TARGET cpp_code)
  add_subdirectory(cpp)
endif() 

include_directories(cpp/include)

find_package(pybind11 REQUIRED)

MESSAGE( [MAIN] "Found pybind11 v${pybind11_VERSION}: ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_INCLUDE_DIRS = ${pybind11_INCLUDE_DIRS}")
MESSAGE( [Main] " pybind11_LIBRARIES = ${pybind11_LIBRARIES}")

pybind11_add_module(
  ${PROJECT_NAME}
  ${PYTHON_SOURCES}
  ${CPP_SOURCES}
)

find_package(OpenMP)

if(OpenMP_CXX_FOUND)
    target_link_libraries( ${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()