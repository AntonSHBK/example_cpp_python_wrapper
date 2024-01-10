cmake_minimum_required(VERSION 3.26.5 FATAL_ERROR)

# set(projectName cpp_code)
project(
    cpp_code 
    VERSION 0.1.0
    DESCRIPTION "cpp_to_python description"
)

# Standarts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest) # We include CTest which is part of CMake

# Dirs
set(PROJECT_SOURCE_DIR "src")
set(PROJECT_INCLUDE_DIR "include/my_lib")

# Headers files
set(
    HEADERS 
    ${PROJECT_INCLUDE_DIR}/my_include_file.h
)

# Source files
set(
    SOURCE
    ${PROJECT_SOURCE_DIR}/my_cpp_file.cpp
)

# Add library
add_library(
    ${PROJECT_NAME}
    SHARED
    # STATIC   
    ${HEADERS}
    ${SOURCE}
)

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC  include/)

# # OpenMP
find_package(OpenMP REQUIRED)
if(NOT OpenMP_CXX_FOUND)
	message(SEND_ERROR "Failed to find OpenMP.")
	return()
else()
	include_directories(${OpenMP_INCLUDE_DIRS})
    message("OpenMP FOUND dir is ${OpenMP_INCLUDE_DIRS}.")
endif()

set(CMAKE_CXX_FLAGS "-fopenmp")

# Указание подключаемых библиотек
target_link_libraries(
    ${projectName} 
    OpenMP::OpenMP_CXX 
)

# Tests dirs
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif ()

enable_testing()