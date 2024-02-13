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

class Var;
class LinTerm;
class GenLinExpr;
class Constraint;
typedef std::vector<LinTerm> LinExpr;

class _Var {
public:
  const std::string name;
  double value;
  const bool isFree;

  _Var(std::string name) : name(name), isFree(true) {}
  _Var(std::string name, double value) : name(name), value(value), isFree(false) {}
};

class Var {
  std::shared_ptr<_Var> _var;

public:
  Var(std::string name) : _var(std::make_shared<_Var>(name)) {}
  Var(std::string name, double value) : _var(std::make_shared<_Var>(name, value)) {}

  // for use in std::set
  bool operator<(const Var& other) const {
    return _var->name < other._var->name;
  }

  bool operator==(const Var& other) const {
    return _var->name == other._var->name;
  }

  _Var* operator->() const {
    return _var.get();
  }

  LinTerm operator*(double coef) const;
  GenLinExpr operator+(const LinTerm& other) const;
  GenLinExpr operator-(const LinTerm& other) const;
  Constraint operator|=(const Var& other) const;
  operator LinTerm() const;

  Constraint operator|=(const GenLinExpr& other) const;
};

LinTerm operator*(double coef, const Var& var);

typedef std::set<Var> Vars;

static inline Vars makeVars(std::initializer_list<std::string> names) {
  Vars vars;
  for (auto name : names) {
    vars.emplace(name);
  }
  return vars;
}

#define MakeVar_VA_ARGS(...) , ##__VA_ARGS__
#define MakeVar(name, ...) geomui::Var name(#name MakeVar_VA_ARGS(__VA_ARGS__))

class LinTerm {
  // linear term
public:
  double coef;
  Var var;

  LinTerm(double coef, Var var) : coef(coef), var(var) {}

  GenLinExpr operator+(const LinTerm& other) const;
  LinTerm operator-() const;
  operator GenLinExpr() const;
};

class GenLinExpr {
public:
  double constant;
  LinExpr terms;

  GenLinExpr() : constant(0) {}
  GenLinExpr(double constant) : constant(constant) {}
  GenLinExpr(double constant, LinExpr terms) : constant(constant), terms(terms) {}
  
  GenLinExpr operator+(const LinTerm& term) const;
  GenLinExpr operator-(const LinTerm& term) const;
  GenLinExpr operator+(double other) const;

  Constraint operator|=(const GenLinExpr& other) const;
  // Constraint operator|=(const LinExpr& other) const;
  // Constraint operator|=(const Var& other) const;
  // Constraint operator|=(const double other) const;
};

GenLinExpr operator+(const double constant, const GenLinExpr& term);

class Constraint {
  // two linear expressions which need to be equal
public:
  double constant;
  LinExpr terms;

  Constraint(double constant, LinExpr terms) : constant(constant), terms(terms) {}
};

typedef std::vector<Constraint> Problem;

enum class SolutionStatus {
  SUCCESS,
  OVERDETERMINED,
  UNDERDETERMINED,
};

SolutionStatus solve(Problem problem);

} // namespace geomui