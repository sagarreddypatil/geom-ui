#include "geomui.hpp"

#include <iostream>
#include <set>
#include <Eigen/Dense>

namespace geomui::internal {

SolutionStatus Problem::solve() {
    if(isSolved) {
        return SolutionStatus::SUCCESS;
    }

    if(vars.size() == 0) {
        return SolutionStatus::SUCCESS;
    }

    if(equations.size() > vars.size()) {
        return SolutionStatus::OVERDETERMINED;
    }

    if(equations.size() < vars.size()) {
        return SolutionStatus::UNDERDETERMINED;
    }

    Eigen::MatrixXd mat(equations.size(), vars.size());
    Eigen::VectorXd vec(equations.size());

    for (uint i = 0; i < equations.size(); i++) {
        auto& c = equations[i];
        vec(i) = c.constant;

        for (uint j = 0; j < vars.size(); j++) {
            auto var = *(std::next(vars.begin(), j));
            auto it = c.vars.find(var);

            if(it != c.vars.end()) {
                mat(i, j) = it->second; // the coefficient
            } else {
                mat(i, j) = 0; // doesn't appear in the equation
            }
        }
    }

    // TODO: maybe skip checking rank and just solve
    // if current implementation is too slow
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

    isSolved = true;
    return SolutionStatus::SUCCESS;
}

double Var::eval() {
    if(problem == nullptr) {
        throw std::runtime_error("Variable not part of a problem");
    }

    problem->solve();

    return value;
}

} // namespace geomui::internal

namespace geomui {

LinExpr upcast(double constant) {
    return LinExpr(constant);
}

LinExpr upcast(const Var& var) {
    return LinExpr(var);
}

LinExpr upcast(const LinTerm& term) {
    return LinExpr(term);
}

LinExpr upcast(const LinExpr& expr) {
    return expr;
}

} // namespace geomui


geomui::LinTerm operator*(double coef, const geomui::Var& var) {
  return {coef, var};
}

geomui::LinTerm operator-(const geomui::LinTerm& term) {
  return {-term.first, term.second};
}

geomui::LinExpr operator+(const geomui::LinExpr& lhs, const geomui::LinTerm& rhs) {
  geomui::LinExpr new_expr = lhs;
  new_expr.second.push_back(rhs);
  return new_expr;
}

geomui::LinExpr operator-(const geomui::LinExpr& lhs, const geomui::LinTerm& rhs) {
  return lhs + (-rhs);
}
