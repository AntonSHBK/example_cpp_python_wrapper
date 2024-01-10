# Simple cpp porting to python

[Not bad tutorial](https://smyt.ru/blog/sozdaem-s-python-rasshireniya-s-pomshyu-pybind11/?ysclid=lr6j5noobx652421112)

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