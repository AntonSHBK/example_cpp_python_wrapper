// #include <pybind11/pybind11.h>
#include "python_header.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(CppToPython, m) {
    m.def("add", &add); // добавили функцию в модуль
};