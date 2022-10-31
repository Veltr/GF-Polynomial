#include "Polynomial.h"

int main(){
    GF g(3, 2); g.build_GF({1, 2, 2}); g.print_GF(std::cout);
    //GF g(11, 1); g.build_GF({1, 2}); g.print_GF(std::cout);

    PolynomGF p1({g.get_element_by_power(5)});
    PolynomGF p2({g.get_element_by_power(7), g.get_element_by_power(3)});
    auto t = p1.sub(p2);

    std::cout << "Done\n";
}
