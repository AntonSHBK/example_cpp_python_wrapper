#include <string>

#ifndef CAR_H
#define CAR_H

namespace vehicles {

class Motorcycle {

private:

    /// Name
    std::string _name;

public:

    /// Constructor
    Motorcycle(std::string name);

    /// Get motorcycle name
    /// @return Motorcycle name
    std::string get_name() const;

    /// Drive the motorcycle
    void ride() const;
};

}

#endif