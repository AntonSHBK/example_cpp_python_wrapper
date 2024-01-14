#include <string>

#ifndef MY_LIB
#define MY_LIB

namespace talker {

class SomeTalker {

private:
    std::string _text;

public:

    // Constructor
    SomeTalker(std::string text);

    // Get text
    std::string get_text();
    //  Set text
    void set_text(std::string text);

    // Get omp_max_treads
    int get_omp_max_treads();

    // Get text with parallel
    void get_text_parallel(int streams);
};

}

int add(const int &a, const int &b);
#endif