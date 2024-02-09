#include "geomui.hpp"

#include <iostream>
#include <set>
#include <Eigen/Dense>

namespace geomui {

SolutionStatus solve(Problem problem) {
    std::set<std::shared_ptr<Var>> vars;
    for (auto c : problem) {
        for (auto t : c.terms) {
            vars.insert(t.var);
        }
    }

    if(vars.size() == 0) {
        return SolutionStatus::SUCCESS;
    }

    if(problem.size() > vars.size()) {
        return SolutionStatus::OVERDETERMINED;
    }

    Eigen::MatrixXd mat(problem.size(), vars.size());
    Eigen::VectorXd vec(problem.size());

    for (uint i = 0; i < problem.size(); i++) {
        auto c = problem[i];
        for (uint j = 0; j < c.terms.size(); j++) {
            auto t = c.terms[j];
            auto it = std::find(vars.begin(), vars.end(), t.var);
            if (it != vars.end()) {
                int idx = std::distance(vars.begin(), it);
                mat(i, idx) = t.coef;
            }
        }
        vec(i) = c.constant;
    }

    auto decomp = mat.fullPivHouseholderQr();
    uint rank = decomp.rank();

    if(rank < vars.size()) {
        return SolutionStatus::UNDERDETERMINED;
    }

    Eigen::VectorXd sol = decomp.solve(vec);

    for (uint i = 0; i < vars.size(); i++) {
        auto it = std::next(vars.begin(), i);
        auto v = *it;
        v->value = sol(i);
    }

    return SolutionStatus::SUCCESS;
}

}
