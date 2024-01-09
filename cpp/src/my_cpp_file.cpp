#include "../include/my_lib/my_include_file.h"

#include <iostream>

namespace vehicles {

Motorcycle::Motorcycle(std::string name) {
    _name = name;
}

std::string Motorcycle::get_name() const {
    return _name;
}

void Motorcycle::ride() const {
    std::cout << "Zoom Zoom" << std::endl;
}

}