#include "interpreter.hpp"
#include "tokenize.hpp"
#include "expression.hpp"
#include "environment.hpp"
#include "interpreter_semantic_error.hpp"
#include <stack>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <deque>

// Helper function: Recursively parse expressions from tokens
Expression Interpreter::parse_expression(std::deque<std::string>::iterator &current, const std::deque<std::string>::iterator &end) {
    if (current == end) {
        throw InterpreterSemanticError("Unexpected end of input");
    }

    std::string token = *current++;

    if (token == "(") {
        if (current == end) {
            throw InterpreterSemanticError("Unexpected end after '('");
        }

        Expression expr;
        std::string head_token = *current++;
        Atom atom;
        if (!token_to_atom(head_token, atom)) {
            throw InterpreterSemanticError("Invalid token in head: " + head_token);
        }
        expr = Expression(atom);

        while (current != end && *current != ")") {
            expr.tail.push_back(parse_expression(current, end));
        }

        if (current == end || *current != ")") {
            throw InterpreterSemanticError("Expected ')'");
        }
        ++current; // Skip the closing parenthesis
        return expr;
    } else if (token == ")") {
        throw InterpreterSemanticError("Unexpected ')' token");
    } else {
        Atom atom;
        if (!token_to_atom(token, atom)) {
            throw InterpreterSemanticError("Invalid token: " + token);
        }
        return Expression(atom);
    }
}


bool Interpreter::parse(std::istream &expression) noexcept {
    try {
        auto tokens = tokenize(expression);
        if (tokens.empty()) {
            return false;
        }

        // Ensure the statement starts with '(' and ends with ')'
        if (tokens.front() != "(" || tokens.back() != ")") {
            return false;
        }

        auto current = tokens.begin();
        ast = parse_expression(current, tokens.end());

        if (current != tokens.end()) {
            return false; // Extra tokens after valid expression
        }
        return true;
    } catch (const InterpreterSemanticError &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}


Expression Interpreter::eval() {
    if (ast.head.type == NoneType) {
        throw InterpreterSemanticError("Empty AST");
    }
    return eval_expression(ast);
}

// Helper function to evaluate parsed expressions
Expression Interpreter::eval_expression(const Expression &expr) {
    if (expr.head.type == NumberType || expr.head.type == BooleanType) {
        return expr; // Atoms evaluate to themselves
    }

    if (expr.head.type == SymbolType) {
        std::string op = expr.head.value.sym_value;

        if (op == "begin") {
            if (expr.tail.empty()) {
                throw InterpreterSemanticError("begin requires at least one expression");
            }
            Expression result;
            for (const auto &sub_expr : expr.tail) {
                result = eval_expression(sub_expr);
            }
            return result;
        } else if (op == "define") {
            if (expr.tail.size() != 2 || expr.tail[0].head.type != SymbolType) {
                throw InterpreterSemanticError("define requires a symbol and an expression");
            }
            Expression value = eval_expression(expr.tail[1]);
            const Symbol& sym_value = expr.tail[0].head.value.sym_value;

            if (sym_value == "if" || sym_value == "begin" || sym_value == "define" || env.is_symbol_defined(sym_value) || env.is_procedure_defined(sym_value))
            {
                throw InterpreterSemanticError(sym_value + " already defined");
            }
            env.add(sym_value, value);
            return value;
        } else if (op == "if") {
            if (expr.tail.size() != 3) {
                throw InterpreterSemanticError("if requires three expressions");
            }
            Expression condition = eval_expression(expr.tail[0]);
            if (condition.head.type != BooleanType) {
                throw InterpreterSemanticError("if condition must be a boolean");
            }
            return condition.head.value.bool_value ? eval_expression(expr.tail[1]) : eval_expression(expr.tail[2]);
        } 
        
        // Check if the symbol is a variable
        else if (env.is_symbol_defined(op)) {
            return env.get(op);
        } 
        
        // Check if the symbol is a procedure
        else if (env.is_procedure_defined(op)) {
            // Retrieve the procedure function
            auto procedure = env.get_procedure(op);
            
            // Evaluate arguments recursively
            std::vector<Atom> evaluated_args;
            for (const auto &arg_expr : expr.tail) {
                Expression evaluated_expr = eval_expression(arg_expr);

                if (evaluated_expr.head.type == NoneType) {
                    throw InterpreterSemanticError("Invalid argument for procedure: " + op);
                }

                evaluated_args.push_back(evaluated_expr.head);
            }

            // Create an Expression to hold the result

            Expression result;
            procedure(evaluated_args, result); // Call the procedure with evaluated arguments

            return result;
        } 
        
        else {
            throw InterpreterSemanticError("Unknown symbol: " + op);
        }
    }

    throw InterpreterSemanticError("Invalid expression");
}
