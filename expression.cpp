#include "expression.hpp"
#include <cmath>
#include <limits>
#include <cctype>
#include <sstream>
#include <stdexcept>

// chatgpt was used on this file for structure
static bool isNumber(const std::string &token);
static bool isSymbol(const std::string &token);

// Constructor for BooleanType expression
Expression::Expression(bool tf) {
    head.type = BooleanType;
    head.value.bool_value = tf;
}

// Constructor for NumberType expression
Expression::Expression(double num) {
    head.type = NumberType;
    head.value.num_value = num;
}

// Constructor for SymbolType expression
Expression::Expression(const std::string & sym) {
    head.type = SymbolType;
    head.value.sym_value = sym;
}

// Equality operator for comparing two expressions
bool Expression::operator==(const Expression & exp) const noexcept {
    const double EPSILON = 1e-9; // Tolerance for floating-point comparison

    // Check if types are different
    if (head.type != exp.head.type) return false;

    // Compare based on type
    switch (head.type) {
        case NoneType:
            return true; // NoneType expressions are always equal
        case BooleanType:
            return head.value.bool_value == exp.head.value.bool_value;
        case NumberType:
            return std::fabs(head.value.num_value - exp.head.value.num_value) < EPSILON;
        case SymbolType:
            return head.value.sym_value == exp.head.value.sym_value;
        default:
            return false;
    }
}

// Check if a token is a number
static bool isNumber(const std::string &token) {
    if (token.empty()) return false;

    // Check if the token starts with a digit
    if (isdigit(token[0])) {
        return true;
    }
    // Check for optional + or - sign followed by a digit
    else if ((token[0] == '+' || token[0] == '-') && token.length() > 1 && isdigit(token[1])) {
        return true;
    }

    return false;
}

// Check if a token is a symbol
static bool isSymbol(const std::string &token) {
    return !isNumber(token) && !token.empty();
}

// Convert token to an Atom based on its type
bool token_to_atom(const std::string & token, Atom & atom) {
    if (token == "True") {
        atom.type = BooleanType;
        atom.value.bool_value = true;
    } 
    else if (token == "False") {
        atom.type = BooleanType;
        atom.value.bool_value = false;
    } 
    // Check if the token is a symbol before checking if it's a number
    else if (isSymbol(token)) {
        atom.type = SymbolType;
        atom.value.sym_value = token;
    } 
    // Check if the token is a number
    else {
        size_t pos = 0;
        try {
            double num = std::stod(token, &pos);
            // Ensure the entire token was parsed successfully
            if (pos == token.length()) {
                atom.type = NumberType;
                atom.value.num_value = num;
                return true;
            }
        } catch (const std::exception &) {
            return false; // Return false if the token is invalid
        }

        return false;
    }

    return true;
}

// Parse a token and return an Expression based on its type
Expression parse_atom(const std::string &token) {
    Atom atom;
    if (!token_to_atom(token, atom)) {
        throw std::invalid_argument("Invalid token: " + token);
    }

    switch (atom.type) {
        case BooleanType:
            return Expression(atom.value.bool_value);
        case NumberType:
            return Expression(atom.value.num_value);
        case SymbolType:
            return Expression(atom.value.sym_value);
        case NoneType:
        default:
            return Expression(); // Return an empty expression if type is None
    }
}

// Overload the << operator for printing an expression
std::ostream & operator<<(std::ostream & out, const Expression & exp) {
    switch (exp.head.type) {
        case NoneType:
            out << "None";
            break;
        case BooleanType:
            out << (exp.head.value.bool_value ? "True" : "False");
            break;
        case NumberType:
            out << exp.head.value.num_value;
            break;
        case SymbolType:
            out << exp.head.value.sym_value;
            break;
        default:
            out << "Unknown";
            break;
    }
    return out;
}
