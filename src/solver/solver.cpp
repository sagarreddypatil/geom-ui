#include "geomui.hpp"

#include <iostream>
#include <set>
#include <Eigen/Dense>

namespace geomui {

LinTerm Var::operator*(double coef) const {
    return LinTerm(coef, *this);
}

GenLinExpr Var::operator+(const LinTerm& other) const{
    return LinTerm(1, *this) + other;
}

GenLinExpr Var::operator-(const LinTerm& other) const {
    return *this + (-other);
}

Constraint Var::operator|=(const Var& other) const {
    return (GenLinExpr)(LinTerm)(*this) |= (LinTerm)other;
}

LinTerm operator*(double coef, const Var& var) {
    return LinTerm(coef, var);
}

Var::operator LinTerm() const {
    return LinTerm(1, *this);
}

Constraint Var::operator|=(const GenLinExpr& other) const {
    return (GenLinExpr)(LinTerm)(*this) |= other;
}

GenLinExpr LinTerm::operator+(const LinTerm& other) const {
    return GenLinExpr(0, {*this, other});
}

LinTerm LinTerm::operator-() const {
    return LinTerm(-coef, var);
}

LinTerm::operator GenLinExpr() const {
    return GenLinExpr(0, {*this});
}

GenLinExpr GenLinExpr::operator+(const LinTerm& term) const {
    LinExpr new_terms = terms;
    new_terms.push_back(term);
    return GenLinExpr(constant, new_terms);
}

GenLinExpr GenLinExpr::operator-(const LinTerm& term) const {
    return *this + (-term);
}

GenLinExpr GenLinExpr::operator+(double other) const {
    return GenLinExpr(this->constant + other, this->terms);
}

GenLinExpr operator+(const double constant, const GenLinExpr& term) {
    return GenLinExpr(constant + term.constant, term.terms);
}

Constraint GenLinExpr::operator|=(const GenLinExpr& other) const {
    LinExpr nterms = terms;
    for(auto t : other.terms) {
        nterms.push_back(LinTerm(-t.coef, t.var));
    }

    double constant = other.constant - this->constant;

    return Constraint(constant, nterms);
}

// Constraint GenLinExpr::operator|=(const LinExpr& other) const {
//     return (GenLinExpr)(*this) |= other + 0;
// }

// Constraint GenLinExpr::operator|=(const Var& other) const;
// Constraint GenLinExpr::operator|=(const double other) const;

SolutionStatus solve(Problem problem) {
    std::set<Var> vars;
    for (auto c : problem) {
        for (auto t : c.terms) {
            if(!t.var->isFree)
                continue;

            vars.insert(t.var);
        }
    }

    if(vars.size() == 0) {
        return SolutionStatus::SUCCESS;
    }

    if(problem.size() > vars.size()) {
        return SolutionStatus::OVERDETERMINED;
    }

    if(problem.size() < vars.size()) {
        return SolutionStatus::UNDERDETERMINED;
    }

    Eigen::MatrixXd mat(problem.size(), vars.size());
    Eigen::VectorXd vec(problem.size());

    for (uint i = 0; i < problem.size(); i++) {
        auto c = problem[i];
        vec(i) = c.constant;

        for (uint j = 0; j < c.terms.size(); j++) {
            auto t = c.terms[j];

            if(!t.var->isFree) {
                auto const_val = t.coef * t.var->value;
                vec(i) -= const_val;
                continue;
            }

            auto it = std::find(vars.begin(), vars.end(), t.var);
            if (it != vars.end()) {
                int idx = std::distance(vars.begin(), it);
                
                mat(i, idx) = t.coef;
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

    return SolutionStatus::SUCCESS;
}

}
