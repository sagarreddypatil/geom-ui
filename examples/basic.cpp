#include <iostream>

#include <geomui.hpp>

int main() {
    /*
     * Let's model this system of equations:
     * x - 2y + 3z = 7
     * 2x + y + z = 4
     * -3x + 2y - 2z = -10
    */

    auto vars = geomui::makeVars({"x", "y", "z"});
    auto x = *vars.begin();
    auto y = *std::next(vars.begin(), 1);
    auto z = *std::next(vars.begin(), 2);

    // geomui::Constraint eq1(7, geomui::LinExpr({geomui::LinTerm(1, x), geomui::LinTerm(-2, y), geomui::LinTerm(3, z)}));
    // geomui::Constraint eq2(4, geomui::LinExpr({geomui::LinTerm(2, x), geomui::LinTerm(1, y), geomui::LinTerm(1, z)}));
    // geomui::Constraint eq3(-10, geomui::LinExpr({geomui::LinTerm(-3, x), geomui::LinTerm(2, y), geomui::LinTerm(-2, z)}));

    geomui::Constraint eq1(vars, "x - 2y + 3z = 7");
    geomui::Constraint eq2(vars, "2x + y + z = 4");
    geomui::Constraint eq3(vars, "-3x + 2y - 2z = -10");

    geomui::Problem problem({eq1, eq2, eq3});

    geomui::solve(problem);

    std::cout << "x = " << x->value << std::endl;
    std::cout << "y = " << y->value << std::endl;
    std::cout << "z = " << z->value << std::endl;

    return 0;
}


