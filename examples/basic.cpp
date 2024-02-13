#include <iostream>

#include <geomui.hpp>

int main() {
    geomui::Var x;
    geomui::Var y;
    geomui::Var z;

    x - 2*y + 3*z |= 7;
    2*x + y + z |= 4;
    -3*x + 2*y - (2*z) |= -10;

    auto& prob = x->problem;

    for(uint i = 0; i < 1000000; i++) {
        prob->invalidate();
        prob->solve();
        // std::cout << "x: " << x->eval() << std::endl;
        // std::cout << "y: " << y->eval() << std::endl;
        // std::cout << "z: " << z->eval() << std::endl;

        // if(i % 1000 == 0) 
        //     std::cout << i << std::endl;
    }

    return 0;
}


