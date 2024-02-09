#pragma once

#include <string>
#include <vector>
#include <set>
#include <memory>

/*
 * 2D Geometric Constraint Solver
 * Limited to Vertical and Horizontal Lines
 * Just need to solve system of linear equations
 */

namespace geomui {

class Var {
public:
  const std::string name;
  double value;
  const bool isFree;

  Var(std::string name) : name(name), isFree(true) {}
  Var(std::string name, double value) : name(name), value(value), isFree(false) {}
  
  inline int operator=(double value) {
    return this->value = value;
  }
};

typedef std::set<std::shared_ptr<Var>> Vars;

static inline Vars makeVars(std::initializer_list<std::string> names) {
  Vars vars;
  for (auto name : names) {
    vars.insert(std::make_shared<Var>(name));
  }
  return vars;
}

#define MakeVar_VA_ARGS(...) , ##__VA_ARGS__
#define MakeVar(name, ...) auto name = std::make_shared<geomui::Var>(#name MakeVar_VA_ARGS(__VA_ARGS__))

class LinTerm {
  // linear term
public:
  double coef;
  std::shared_ptr<Var> var;

  LinTerm(double coef, std::shared_ptr<Var> var) : coef(coef), var(var) {}
};

typedef std::vector<LinTerm> LinExpr;

class Constraint {
  // two linear expressions which need to be equal
public:
  double constant;
  LinExpr terms;

  Constraint(double constant, LinExpr terms) : constant(constant), terms(terms) {}
  Constraint(Vars vars, std::string expr); // parse string
};

typedef std::vector<Constraint> Problem;

enum class SolutionStatus {
  SUCCESS,
  OVERDETERMINED,
  UNDERDETERMINED,
};

SolutionStatus solve(Problem problem);

} // namespace geomui