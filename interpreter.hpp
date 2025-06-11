#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "expression.hpp"
#include "environment.hpp"
#include <istream>
#include <deque>
#include <string>

// Interpreter class to parse and evaluate expressions
class Interpreter {
public:
    // Default constructor
    Interpreter() = default;

    // Parses an expression from the input stream
    bool parse(std::istream &expression) noexcept;

    // Evaluates the parsed expression and returns the result
    Expression eval();

private:
    Expression ast;  // Abstract Syntax Tree (AST) representing the parsed expression
    Environment env; // Environment to store symbols and procedures

    // Parses an expression from a deque of tokens
    Expression parse_expression(std::deque<std::string>::iterator &current, const std::deque<std::string>::iterator &end);

    // Evaluates a given expression
    Expression eval_expression(const Expression &expr);
};

#endif
