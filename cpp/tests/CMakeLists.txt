cmake_minimum_required(VERSION 3.26.5 FATAL_ERROR)

set(projectName test_cpp)
project(
    ${projectName} 
    VERSION 0.1.0
    DESCRIPTION "test_cpp description"
)

# Standarts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${projectName} src/test_cpp.cpp)
target_link_libraries(${projectName} PUBLIC cpp_code)
add_test(NAME test_cpp COMMAND test_cpp)