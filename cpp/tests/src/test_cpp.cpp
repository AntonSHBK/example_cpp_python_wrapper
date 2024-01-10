#include "my_lib/my_include_file.h"
#include <iostream>

int main() {

    talker::SomeTalker my_talker("Hello world");

    std::cout << "Max threads is " << my_talker.get_omp_max_treads() << std::endl;

    my_talker.get_text_parallel(6);

    return 0;
}