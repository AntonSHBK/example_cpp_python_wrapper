#include <iostream>
#include <omp.h>

#include "my_lib/my_include_file.h"

namespace talker {

SomeTalker::SomeTalker(std::string text) {
    _text = text;
}

std::string SomeTalker::get_text() {
    return _text;
}

void SomeTalker::set_text(std::string text) {
    _text = text;
}

int SomeTalker::get_omp_max_treads() {
    return omp_get_max_threads();
}

void SomeTalker::get_text_parallel(int streams = 4) {
    #pragma omp parallel for
    for (int i = 0; i < streams; i++)
    {
        std::cout << omp_get_thread_num() << _text;
    }
    std::cout << std::endl;
}

}

int add(const int &a, const int &b) {
    return a + b;
}