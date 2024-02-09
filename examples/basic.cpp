#include <iostream>

#include <geomui.hpp>

int main() {
    // MakeVar(Ax, 0);
    // MakeVar(Ay, 0);
    // MakeVar(Bx);
    // MakeVar(By);
    // MakeVar(Cx);
    // MakeVar(Cy);
    // MakeVar(width, 10);
    // MakeVar(height, 5);

    // auto vars = {Ax, Ay, Bx, By, Cx, Cy, width, height};

    // auto eq1 = Ay |= By;
    // auto eq2 = Ax |= Cx;
    // auto eq3 = width |= Bx - Ax;
    // auto eq4 = height |= Cy - Ay;

    // geomui::Problem problem({eq1, eq2, eq3, eq4});

    MakeVar(x);
    MakeVar(y);
    MakeVar(z);

    auto vars = {x, y, z};

    geomui::Problem problem({
        x - 2*y + 3*z |= 7,
        2*x + y + z |= 4,
        -3*x + 2*y - (2*z) |= -10

    });

    auto status = geomui::solve(problem);
    if(status == geomui::SolutionStatus::OVERDETERMINED) {
        std::cout << "Overdetermined system" << std::endl;
        return 1;
    } else if(status == geomui::SolutionStatus::UNDERDETERMINED) {
        std::cout << "Underdetermined system" << std::endl;
        return 1;
    }

    // print out the solution
    for (auto v : vars) {
        std::cout << v->name << " = " << v->value << std::endl;
    }

    return 0;
}


