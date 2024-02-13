#include <iostream>

#include <geomui.hpp>

int main() {
    geomui::Var x;
    geomui::Var y;
    // geomui::Var x(10);
    // geomui::Var y(2);
    geomui::Var z;

    x - 2*y + 3*z |= 7;
    2*x + y + z |= 4;
    -3*x + 2*y - (2*z) |= -10;

    auto& prob = x->problem;

    auto status = prob->solve();
    if(status == geomui::SolutionStatus::UNDERDETERMINED) {
        std::cout << "Underdetermined" << std::endl;
    } else if(status == geomui::SolutionStatus::OVERDETERMINED) {
        std::cout << "Overdetermined" << std::endl;
    } else {
        std::cout << "Success" << std::endl;
    }

    std::cout << "x: " << x->eval() << std::endl;
    std::cout << "y: " << y->eval() << std::endl;
    std::cout << "z: " << z->eval() << std::endl;

    return 0;
}


