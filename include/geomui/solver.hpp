#pragma once

#include <vector>
#include <unordered_map>
#include <set>
#include <variant>
#include <memory>
#include <stdexcept>

/*
 * 2D Geometric Constraint Solver
 * Limited to Vertical and Horizontal Lines
 * Just need to solve system of linear equations
 */

namespace geomui {

enum class SolutionStatus {
  SUCCESS,
  OVERDETERMINED,
  UNDERDETERMINED
};

} // namespace geomui

namespace geomui::internal {

class Problem;

class Var {
public:
  double value;

  const bool isFree;
  std::shared_ptr<Problem> problem;

public:
  Var() : value(0), isFree(true), problem(nullptr) {}
  Var(double value) : value(value), isFree(false), problem(nullptr) {}

  double eval();
};

class Equation {
public:
  std::unordered_map<std::shared_ptr<Var>, double> vars;
  double constant = 0; // rhs constant

  Equation() = default;

  void addTerm(double coeff, std::shared_ptr<Var> var, bool left) {
    auto inserted = vars.insert({var, 0}).first;

    const double accCoeff = left ? coeff : -coeff;
    inserted->second += accCoeff;
  }

  void addConstant(double constant, bool left) {
    const double accConstant = left ? -constant : constant;
    this->constant += accConstant;
  }
};

class Problem {
  bool isSolved = false;
public:
  std::vector<Equation> equations;
  std::set<std::shared_ptr<Var>> vars;

  Problem() = default;

  void addEquation(const Equation& eq) {
    equations.push_back(eq);
    for (auto& [var, _] : eq.vars) {
      vars.insert(var);
    }

    isSolved = false;
  }

  SolutionStatus solve();

  void invalidate() {
    isSolved = false;
  }
};

} // namespace geomui::internal

namespace geomui {

// typedef std::shared_ptr<internal::Var> Var;
// typedef std::pair<double, Var> LinTerm;
// typedef std::pair<double, std::vector<LinTerm>> LinExpr;

class Var {
private:
  std::shared_ptr<internal::Var> var;
public:
  Var() : var(std::make_shared<internal::Var>()) {}
  Var(double value) : var(std::make_shared<internal::Var>(value)) {}

  Var(const Var& other) : var(other.var) {}
  Var(Var&& other) : var(std::move(other.var)) {}

  internal::Var* operator->() const {
    return var.get();
  }

  operator std::shared_ptr<internal::Var>() {
    return var;
  }

  // for std::set
  bool operator<(const Var& other) const {
    return var < other.var;
  }
};

class LinTerm : public std::pair<double, Var> {
public:
  LinTerm(double coef, Var var) : std::pair<double, Var>(coef, var) {}
  LinTerm(Var var) : LinTerm(1, var) {}
};

class LinExpr : public std::pair<double, std::vector<LinTerm>> {
public:
  LinExpr(double constant, const std::vector<LinTerm>& terms)
      : std::pair<double, std::vector<LinTerm>>(constant, terms) {}
  LinExpr(double constant) : LinExpr(constant, {}) {}
  LinExpr(const LinTerm& term) : LinExpr(0, {term}) {}
  LinExpr(const Var& var) : LinExpr(0, {LinTerm(var)}) {}
};

LinExpr upcast(double constant);
LinExpr upcast(const Var& var);
LinExpr upcast(const LinTerm& term);
LinExpr upcast(const LinExpr& expr);

} // namespace geomui

template<typename Lhs, typename Rhs>
static void operator|=(const Lhs& lhs, const Rhs& rhs) {
  geomui::LinExpr lhsexpr = geomui::upcast(lhs);
  geomui::LinExpr rhsexpr = geomui::upcast(rhs);

  std::set<geomui::Var> vars;
  for(auto& [_, v] : lhsexpr.second) {
    vars.insert(v);
  }
  for(auto& [_, v] : rhsexpr.second) {
    vars.insert(v);
  }

  // check if any of the vars have an associated problem
  std::set<std::shared_ptr<geomui::internal::Problem>> problems;
  for(auto& v : vars) {
    if(v->problem) {
      problems.insert(v->problem);
    }
  }

  // combine all the problems
  auto problem = std::make_shared<geomui::internal::Problem>();
  for(auto& p : problems) {
    for(auto& eq : p->equations) {
      problem->addEquation(eq);
    }
  }

  // set all vars of the new problem to be part of the problem
  for(auto& v : problem->vars) {
    v->problem = problem;
  }

  // set all vars to be part of the same problem
  for(auto& v : vars) {
    v->problem = problem;
  }

  // construct an equation and add it to the problem
  geomui::internal::Equation eq;
  eq.addConstant(lhsexpr.first, true);
  for(auto& [c, v] : lhsexpr.second) {
    eq.addTerm(c, v, true);
  }

  eq.addConstant(rhsexpr.first, false);
  for(auto& [c, v] : rhsexpr.second) {
    eq.addTerm(c, v, false);
  }

  // add the equation to the problem
  problem->addEquation(eq);

  // return the problem
  // return problem;
}

geomui::LinTerm operator*(double coef, const geomui::Var& var);
geomui::LinTerm operator-(const geomui::LinTerm& term);
geomui::LinExpr operator+(const geomui::LinExpr& lhs, const geomui::LinExpr& rhs);
geomui::LinExpr operator-(const geomui::LinExpr& lhs, const geomui::LinExpr& rhs);

