#include "geomui.hpp"

#include <iostream>
#include <set>
#include <Eigen/Dense>

namespace geomui {

void system::solve() {
    std::vector<variable> freeVars;
    std::set<std::shared_ptr<double>> freeVarSet;

    for (auto& c : constraints) {
        for (auto &term : c.lhs.terms) {
            if(!term.var.has_value()) {
                continue;
            }

            auto var = term.var.value();
            if (freeVarSet.find(var.value) == freeVarSet.end()) {
                freeVars.push_back(var);
                freeVarSet.insert(var.value);
            }
        }
    }

    // all equations are in the form of c1*x1 + c2*x2 + ... + cn*xn == constant
    //                               or c1*x1 + c2*x2 + ... + cn*xn >= constant

    // need to convert this to Ax = b
    // and x >= 0

    uint numSlack = 0;
    for (auto& c : constraints) {
        numSlack += c.inequality;
    }

    Eigen::MatrixXd mat(constraints.size(), freeVars.size() + numSlack);
    Eigen::VectorXd vec(constraints.size());

    for (uint i = 0; i < constraints.size(); i++) {
        auto& c = constraints[i];
        vec(i) = c.rhs;

        for (uint j = 0; j < freeVars.size(); j++) {
            const auto var = *(std::next(freeVars.begin(), j));
            const auto it = c.vars.find(var);

            if(it != c.vars.end()) {
                mat(i, j) = it->second; // the coefficient
            } else {
                mat(i, j) = 0; // doesn't appear in the equation
            }
        }

        for (auto& v : fixedVars) {
            auto it = c.vars.find(v);
            if(it != c.vars.end()) {
                vec(i) -= it->second * v->value;
            }
        }
    }

    // TODO: maybe skip checking rank and just solve
    // if current implementation is too slow
    auto decomp = mat.fullPivHouseholderQr();
    // uint rank = decomp.rank();

    // if(rank < freeVars.size()) {
    //     return SolutionStatus::UNDERDETERMINED;
    // }

    Eigen::VectorXd sol = decomp.solve(vec);

    for (uint i = 0; i < freeVars.size(); i++) {
        auto it = std::next(freeVars.begin(), i);
        auto v = *it;
        v->value = sol(i);
    }

    isSolved = true;
    return SolutionStatus::SUCCESS;
}

} // namespace geomui
