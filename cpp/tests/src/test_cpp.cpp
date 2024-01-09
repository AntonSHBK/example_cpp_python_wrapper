#include "my_lib/my_include_file.h"
#include <omp.h>
#include <iostream>

int main() {
    std::cout << omp_get_max_threads() << std::endl;
#pragma omp parallel for
    for (int i = 0; i < 5; i++)
    {
        std::cout << omp_get_thread_num();
    }

    vehicles::Motorcycle c("Yamaha");

    std::cout << "Made a motorcycle called: " << c.get_name() << std::endl;

    c.ride();

    return 0;
}