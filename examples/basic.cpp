#include <iostream>

#include <geomui.hpp>

int main() {
    MakeVar(Ax, 0);
    MakeVar(Ay, 0);
    MakeVar(Bx);
    MakeVar(By);
    MakeVar(Cx);
    MakeVar(Cy);
    MakeVar(width, 10);
    MakeVar(height, 5);

    auto vars = {Ax, Ay, Bx, By, Cx, Cy, width, height};

    geomui::Constraint eq5(vars, "Ay = By");
    geomui::Constraint eq6(vars, "Ax = Cx");
    geomui::Constraint eq7(vars, "width = Bx - Ax");
    geomui::Constraint eq8(vars, "height = Cy - Ay");

    geomui::Problem problem({eq5, eq6, eq7, eq8});

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


