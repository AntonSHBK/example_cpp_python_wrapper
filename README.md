# Simple cpp porting to python

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