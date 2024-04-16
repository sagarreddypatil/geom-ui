#pragma once

#include <vector>
#include <set>
#include <memory>
#include <optional>

/*
 * 2D Geometric Constraint Solver
 * Limited to Vertical and Horizontal Lines
 * Just need to solve system of linear equations
 */

namespace geomui {

class variable;
class expression;

class variable {
public:
  std::shared_ptr<double> value;
  bool isFree;

  variable() : value(std::make_shared<double>(0)), isFree(true) {}
  variable(double value) : value(std::make_shared<double>(value)), isFree(false) {}
  variable(const variable &other) : value(other.value), isFree(other.isFree) {}

  operator expression();
};

class expression {
public:
  class term {
  public:
    std::optional<variable> var;
    double coef;

    term(variable var, double coef) : var(var), coef(coef) {}
    term(double coef) : var(std::nullopt), coef(coef) {}
  };

  std::vector<term> terms;

  expression() {}
  expression(const variable &var) { terms.push_back(term(var, 1)); }
  expression(double coef) { terms.push_back(term(coef)); }
  expression(const expression &other) : terms(other.terms) {}

  inline expression &operator+=(const expression &other) {
    for (auto &term : other.terms) {
      terms.push_back(term);
    }
    return *this;
  }

  inline expression &operator*=(double coef) {
    for (auto &term : terms) {
      term.coef *= coef;
    }
    return *this;
  }

  inline expression &operator/=(double coef) {
    for (auto &term : terms) {
      term.coef /= coef;
    }
    return *this;
  }

  inline expression operator-() const {
    expression ret = *this;
    for (auto &term : ret.terms) {
      term.coef = -term.coef;
    }
    return ret;
  }
};

inline expression operator*(double coef, const expression &expr) {
  expression ret = expr;
  ret *= coef;
  return ret;
};

inline expression operator*(const expression &expr, double coef) {
  expression ret = expr;
  ret *= coef;
  return ret;
};

inline expression operator/(const expression &expr, double coef) {
  expression ret = expr;
  ret /= coef;
  return ret;
};

inline expression operator+(const expression &lhs, const expression &rhs) {
  expression ret = lhs;
  ret += rhs;
  return ret;
};

inline expression operator+(double coef, const expression &expr) {
  expression ret = expr;
  ret += coef;
  return ret;
};

inline expression operator+(const expression &expr, double coef) {
  expression ret = expr;
  ret += coef;
  return ret;
};

class constraint {
  public:
    expression lhs; // reduced form, no constants
    double rhs; // constant

    // always in the form of lhs <= rhs or lhs == rhs
    // lhs >= rhs can be transformed to -lhs <= -rhs
    bool inequality; // represents if >= (true) or == (false)
  constraint(expression lhs, bool inequality) : lhs(lhs), inequality(inequality) {}
};

class system {
private:
  std::vector<constraint> constraints;

public:
  system(std::initializer_list<constraint> constraints) : constraints(constraints) {}
  void solve();
};

} // namespace geomui
