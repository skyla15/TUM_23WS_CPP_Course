#include "combinatorics.h"
#include "hw02.h"

#include <iostream>


int main() {
    // here you can test your functions
    std::cout << "perm(10, 3)=" << permutation(10, 3) << std::endl;
    std::cout << "perm(20, 10)=" << permutation(20, 10) << std::endl;
    std::cout << "perm(20, 20)=" << permutation(20, 20) << std::endl;
    
    std::cout << "comb(10, 3)=" << combination(10, 3) << std::endl;
    std::cout << "comb(20, 10)=" << combination(20, 10) << std::endl;
    std::cout << "comb(10, 10)=" << combination(10, 10) << std::endl;
    return 0;
}
