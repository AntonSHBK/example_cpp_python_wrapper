#include <pybind11/pybind11.h>

// #include "python_header.h"

namespace py = pybind11;

int add(const int &a, const int &b) {
    return a + b;
}

PYBIND11_MODULE(CppToPython, m) {
    m.def("add", &add); // добавили функцию в модуль
};