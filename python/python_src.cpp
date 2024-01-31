#include <omp.h>
#include <pybind11/pybind11.h>

#include <my_lib/my_include_file.h>
#include "python_header.h"

namespace py = pybind11;

int add(const int &a, const int &b) {
    return a + b;
}


PYBIND11_MODULE(CppToPython, m) {
    m.def("add", &add); // добавили функцию выше
    m.def("bob", &talker::bob); // добавили функцию из mi_lib

    py::class_<talker::SomeTalker>(m, "SomeTalker")
        .def(py::init<std::string>())
        .def("get_omp_max_treads", &talker::SomeTalker::get_omp_max_treads)
        .def("get_text_parallel", &talker::SomeTalker::get_text_parallel);

    m.def("get_max_threads", &omp_get_max_threads, "Returns max number of threads");
    m.def("set_num_threads", &omp_set_num_threads, "Set number of threads");
};