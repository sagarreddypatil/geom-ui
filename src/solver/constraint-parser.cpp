#include "geomui.hpp"

#include <sstream>
#include <functional>
#include <variant>
#include <iostream>

namespace geomui {

/*
 * Grammar for Constraint Expressions
 * side ::= <fterm> [<term>]+
 * fterm ::= <sign>? <coef>? <var>
 * term ::= <sign> <coef>? <var>
 * sign ::= '+' | '-'
 * coef ::= <num>
 * var ::= <alphanum>+
 * alphanum ::= [a-zA-Z0-9_]
 * num ::= [0-9]+('.'[0-9]+)?
 *
 * expr ::= <side> '=' <side>
 */

class Token {
public:
  enum class Type {
    SIGN,
    EQUAL,
    NUM,
    ALPHANUM,
    END
  };

  Type type;
  std::string value;
  uint pos;

  Token(Type type, std::string value, uint pos) : type(type), value(value), pos(pos) {}
};

bool isSpace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

bool isPlus(char c) { return c == '+'; }

bool isMinus(char c) { return c == '-'; }

bool isSign(char c) { return isPlus(c) || isMinus(c); }

bool isEqual(char c) { return c == '='; }

bool isDigit(char c) { return c >= '0' && c <= '9'; }

bool isAlphaNum(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || isDigit(c) ||
         c == '_';
}

bool isDot(char c) { return c == '.'; }

bool isNumOrVar(Token& token) {
  return token.type == Token::Type::NUM || token.type == Token::Type::ALPHANUM;
}

bool isVarOrSign(Token& token) {
  return token.type == Token::Type::ALPHANUM || token.type == Token::Type::SIGN;
}

bool isSignOrEqualOrEnd(Token& token) {
  return token.type == Token::Type::SIGN || token.type == Token::Type::EQUAL || token.type == Token::Type::END;
}

bool isSignToken(Token& token) {
  return token.type == Token::Type::SIGN;
}

bool isEqualToken(Token& token) {
  return token.type == Token::Type::EQUAL;
}

bool isNum(Token& token) {
  return token.type == Token::Type::NUM;
}

bool isVar(Token& token) {
  return token.type == Token::Type::ALPHANUM;
}

class Lexer {
  std::string input;
  uint pos;

  char peek;

  char get() { 
    peek = input[pos + 1];
    return input[pos++];
  }

  char expect(std::function<bool(char)> pred, std::string msg) {
    if (!pred(peek)) {
      std::stringstream ss;
      ss << input << "\n";
      for (uint i = 0; i < pos; i++) {
        ss << " ";
      }
      ss << "^" << std::endl;
      ss << "Expected " << msg << " at position, got " << peek;
      throw std::runtime_error(ss.str());
    }
    return get();

  }

public:
  Lexer(std::string input) : input(input), pos(0), peek(input[0]) {
    tokenPeek = _next();
  }

  Token tokenPeek = Token(Token::Type::END, "", 0);

private:
  Token _next() {
    if(pos >= input.size()) {
      return Token(Token::Type::END, "", pos);
    }

    while (isSpace(peek)) get();

    if (isSign(peek)) {
      return Token(Token::Type::SIGN, std::string(1, get()), pos);
    }

    if (isEqual(peek)) {
      return Token(Token::Type::EQUAL, std::string(1, get()), pos);
    }

    if (isDigit(peek)) {
      std::string num;

      num += get();
      uint numPos = pos;

      while (isDigit(peek)) {
        num += get();
      }

      if (isDot(peek)) {
        num += get();

        num += expect(isDigit, "digit");
        while (isDigit(peek)) {
          num += get();
        }
      }

      return Token(Token::Type::NUM, num, numPos);
    }

    if (isAlphaNum(peek)) {
      std::string alphanum;

      alphanum += get();
      uint alphanumPos = pos;

      while (isAlphaNum(peek)) {
        alphanum += get();
      }

      return Token(Token::Type::ALPHANUM, alphanum, alphanumPos);
    }

    throw std::runtime_error("Unexpected character " + std::string(1, peek));
  }

public:
  Token next() {
    auto temp = tokenPeek;
    tokenPeek = _next();
    return temp;
  }

  Token expect(std::function<bool(Token&)> pred, std::string msg) {
    if (!pred(tokenPeek)) {
      std::stringstream ss;
      ss << "\n" << input << "\n";
      for (uint i = 0; i < tokenPeek.pos; i++) {
        ss << " ";
      }
      ss << "^" << std::endl;
      ss << "Expected " << msg << " at position, got " << tokenPeek.value;

      throw std::runtime_error(ss.str());
    }

    return next();
  }

};

double parseSignOptional(Lexer& lexer) {
  Token token = lexer.tokenPeek;

  if(token.type == Token::Type::SIGN) {
    token = lexer.next();
    if(token.value == "-") {
      return -1;
    }
  }
  return 1;
}

double parseSign(Lexer& lexer) {
  auto token = lexer.expect(isSignToken, "sign");
  if(token.value == "-") {
    return -1;
  }
  return 1;
}

std::variant<double, LinTerm> parseTermAfterSign(double coeff, Lexer& lexer, Vars vars) {
  Token token = lexer.tokenPeek;

  if(token.type == Token::Type::NUM) {
    token = lexer.next(); // doesn't change token, only advances
    coeff *= std::stod(token.value);

    token = lexer.next();
  }
  else {
    token = lexer.expect(isVar, "variable");
  }

  // all LHS terms are positive, and all LHS constants are negative
  if(!isSignOrEqualOrEnd(token)) {
    std::string varname = token.value;

    auto var = std::find_if(vars.begin(), vars.end(), [&varname](auto v) {
      return v->name == varname;
    });

    if(var == vars.end()) {
      throw std::runtime_error("Variable " + varname + " not found");
    }

    return LinTerm(coeff, *var);
  }
  else {
    // next token must be a sign, so this is a constant term
    return coeff;
  }
}

std::tuple<double, LinExpr> parseSide(Lexer& lexer, Vars vars, Token::Type endType = Token::Type::END) {
  LinExpr sideTerms;
  double sideConsts = 0;

  double curCoef = parseSignOptional(lexer);
  auto firstTerm = parseTermAfterSign(curCoef, lexer, vars);
  if (std::holds_alternative<LinTerm>(firstTerm)) {
    sideTerms.push_back(std::get<LinTerm>(firstTerm));
  }
  else {
    sideConsts += std::get<double>(firstTerm);
  }

  while (lexer.tokenPeek.type != endType) {
    curCoef = parseSign(lexer);
    auto term = parseTermAfterSign(curCoef, lexer, vars);

    if (std::holds_alternative<LinTerm>(term)) {
      sideTerms.push_back(std::get<LinTerm>(term));
    }
    else {
      sideConsts += std::get<double>(term);
    }
  }

  return std::make_tuple(sideConsts, sideTerms);
}

Constraint::Constraint(Vars vars, std::string expr) {
  Lexer lexer(expr);

  auto [lhsConsts, lhsTerms] = parseSide(lexer, vars, Token::Type::EQUAL);
  lexer.next(); // consume equal sign
  auto [rhsConsts, rhsTerms] = parseSide(lexer, vars);

  constant = rhsConsts - lhsConsts;
  terms = lhsTerms;
  for(auto& term : rhsTerms) {
    term.coef *= -1;
    terms.push_back(term);
  }

  // print the parsed constraint
  std::cout << "Input: " << expr << std::endl;
  std::cout << "Parsed: ";
  bool first = true;
  for(auto& term : terms) {
    std::cout << (!first && term.coef >= 0 ? "+ " : "") << term.coef << term.var->name << " ";
    first = false;
  }
  std::cout << "= " << constant << "\n" << std::endl;
}

} // namespace geomui