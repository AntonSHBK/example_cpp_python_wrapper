# Example of wrapping the Cpp library for Python using Pybind11 and OpenMP

The project provides an example of a library written in C++ with the CMake build system, as well as a code wrapping module for working in Python. The C++ library can be used separately, and you can also use a wrapper for Python.

<img src="https://repository-images.githubusercontent.com/327470624/2d865200-6bf1-11eb-9d0e-2a488615d6ec" alt="Cpp logotype" width="400"/>

<img src="https://qiita-user-contents.imgix.net/https%3A%2F%2Fqiita-image-store.s3.amazonaws.com%2F0%2F234222%2Fcd7d94d8-9a4e-fb0f-a3c2-39deff6fce77.png?ixlib=rb-4.0.0&auto=format&gif-q=60&q=75&s=9bccb26be639078f4e7885e4ee7ea0b0" alt="Pybind11 logotype" width="400"/> 

<img src="https://fuzeservers.ru/wp-content/uploads/f/7/6/f767b64a7d72b5a23b468aa191c085bc.jpeg" alt="Python logotype" width="400"/>

<img src="https://mms.businesswire.com/media/20201113005450/en/307885/22/OpenMPLogo-rgb.jpg" alt="OpenMP logotype" width="400">

## Interesting materials
- [Ideological inspiration](https://github.com/smrfeld/cmake_cpp_pybind11_tutorial)
- [Official tutorial](https://pybind11.readthedocs.io/en/stable/basics.html)
- [Official Pybind11 examples](https://github.com/pybind/cmake_example)
- [Interesting tutorial for pybind11](https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/?ysclid=lr6j5noobx652421112)
- [About CMake expansion setup.py (wrapping)](https://www.benjack.io/hybrid-python/c-packages-revisited/)

## Dependencies
- cmake
- pybind11

## Using C++ library only

```bush
git clone https://github.com/AntonSHBK/simple_cpp_python_porting.git
```

For use in a project in conjunction with Cmake:

```cmake
if (NOT TARGET cpp_code)
    add_subdirectory(cpp)
endif() 
target_link_libraries(${PROJECT_NAME} cpp_code)
```

The project also uses the OpenMP library, this project can serve as an example of use. It is important that the Pybind11 build project has an import of the OpenMP library
```cmake
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries( ${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()
```

### Building project
```bush
mkdir build && cd build
# unix
cmake ..
make
# win (my machine)
cmake .. -G "MinGW Makefiles"
make
```

### Testing

Testing the library's performance. For test i use CTest. 
For verbose output you will use:
```bush
cd build
ctest --verbose
```

## Using Python library

The deployment was tested on a Linux system, for use on Windows, you need to look at the official documentation.

```
pip3 install pybind11
pip3 install . --break-system-packages
```
Run testing library while in the working directory:
```bush
python3 python_test.py
```

```cpp
#include <omp.h>
#include <pybind11/pybind11.h>

#include <my_lib/my_include_file.h>
#include "python_header.h"

namespace py = pybind11;

int add(const int &a, const int &b) {
    return a + b;
}

PYBIND11_MODULE(CppToPython, m) {
    m.def("add", &add); // добавили функцию в модуль
    m.def("bob", &talker::bob); // добавили функцию в модуль

    py::class_<talker::SomeTalker>(m, "SomeTalker")
        .def(py::init<std::string>())
        .def("get_omp_max_treads", &talker::SomeTalker::get_omp_max_treads)
        .def("get_text_parallel", &talker::SomeTalker::get_text_parallel);
};
```

There is a Docker container for testing. Running container:
```bush
cd /docker
docker-compose up --build
```

Result:
```python
import CppToPython

print(CppToPython.add(40, 10))
print(CppToPython.bob(40, 10))

print(CppToPython.sum_thread_ids())
print(CppToPython.get_max_threads())

this_talker = CppToPython.SomeTalker('Hello world!')

print(this_talker.get_omp_max_treads())
this_talker.get_text_parallel(4)
```
