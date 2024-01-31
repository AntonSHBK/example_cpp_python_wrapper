cmake_minimum_required(VERSION 3.25.1 FATAL_ERROR)

project(
    test_cpp 
    VERSION 0.1.0
    DESCRIPTION "test_cpp description"
)

# Standarts
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} src/test_cpp.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC cpp_code)
add_test(NAME test_cpp COMMAND test_cpp)