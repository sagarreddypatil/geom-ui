#include "geomui.hpp"

#include <iostream>
#include <set>
#include <Eigen/Dense>

namespace geomui::internal {

SolutionStatus Problem::solve() {
    if(isSolved) {
        return SolutionStatus::SUCCESS;
    }

    std::set<std::shared_ptr<Var>> freeVars;
    for (auto& v : vars) {
        if(v->isFree) {
            freeVars.insert(v);
        }
    }

    std::set<std::shared_ptr<Var>> fixedVars;
    for (auto& v : vars) {
        if(!v->isFree) {
            fixedVars.insert(v);
        }
    }

    if(freeVars.size() == 0) {
        return SolutionStatus::SUCCESS;
    }

    if(equations.size() > freeVars.size()) {
        std::cout << "Overdetermined. " << equations.size() << " equations, " << freeVars.size() << " free vars" << std::endl;
        return SolutionStatus::OVERDETERMINED;
    }

    if(equations.size() < freeVars.size()) {
        std::cout << "Underdetermined. " << equations.size() << " equations, " << freeVars.size() << " free vars" << std::endl;
        return SolutionStatus::UNDERDETERMINED;
    }
    // std::cout << "Correct. " << equations.size() << " equations, " << freeVars.size() << " free vars" << std::endl;

    Eigen::MatrixXd mat(equations.size(), freeVars.size());
    Eigen::VectorXd vec(equations.size());

    for (uint i = 0; i < equations.size(); i++) {
        auto& c = equations[i];
        vec(i) = c.constant;

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

double Var::eval() {
    if(problem == nullptr) {
        throw std::runtime_error("Variable not part of a problem");
    }

    auto status = problem->solve();
    if(status == SolutionStatus::UNDERDETERMINED) {
        throw std::runtime_error("Underdetermined system of equations");
    }
    if (status == SolutionStatus::OVERDETERMINED) {
        throw std::runtime_error("Overdetermined system of equations");
    }

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

geomui::LinExpr operator+(const geomui::LinExpr& lhs, const geomui::LinExpr& rhs) {
  geomui::LinExpr new_expr = lhs;
  for (auto& term : rhs.second) {
    new_expr.second.push_back(term);
  }
  new_expr.first += rhs.first;
  return new_expr;
}

geomui::LinExpr operator-(const geomui::LinExpr& lhs, const geomui::LinExpr& rhs) {
  geomui::LinExpr new_expr = lhs;
  for (auto& term : rhs.second) {
    new_expr.second.push_back(-term);
  }
  new_expr.first -= rhs.first;
  return new_expr;
}
