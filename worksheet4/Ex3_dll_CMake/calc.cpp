// begin = calc.cpp ============
#include <sstream>
#include <iostream>
#include "adder.h"  // Include the header file for add function

int main(int argc, char* argv[]) {
    int a, b, c;

    if (argc != 3) return 1;

    std::string sa(argv[1]);
    std::string sb(argv[2]);

    std::stringstream ssa(sa);
    std::stringstream ssb(sb);

    ssa >> a;
    ssb >> b;

    c = add(a, b);
    std::cout << c << std::endl;

    return 0;
}
