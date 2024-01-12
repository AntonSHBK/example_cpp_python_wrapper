# Simple cpp porting to python

[Idea](https://github.com/smrfeld/cmake_cpp_pybind11_tutorial)

[Official tutorial](https://pybind11.readthedocs.io/en/stable/basics.html)

[Pybind11 examples](https://github.com/pybind/cmake_example)

[Not bad tutorial for pybind11](https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/?ysclid=lr6j5noobx652421112)

[About CMake expansion setup.py](https://www.benjack.io/hybrid-python/c-packages-revisited/)

## Building cpp
```bush
mkdir build && cd build
# unix
cmake ..
# win (my machine)
cmake .. -G "MinGW Makefiles"
make
```
End next try testing...

## Testing

For test i use CTest. For verbose output you will use:
```bush
cd build
ctest --verbose
```

[Вдохновение статье](https://github.com/smrfeld/cmake_cpp_pybind11_tutorial.git)