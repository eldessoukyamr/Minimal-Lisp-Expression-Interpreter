#define CATCH_CONFIG_MAIN  // Define the main function for Catch2 framework
#define CATCH_CONFIG_COLOUR_NONE // Disable colored output for Catch2 tests
#include "catch.hpp"
#include "interpreter.hpp"
#include "tokenize.hpp"
#include <sstream> // For handling input stream manipulations

// ------------------------------- Interpreter Tests -------------------------------

// Test case for handling division by zero in the interpreter
TEST_CASE("Test division by zero", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(/ 5 0)"); // Input expression that divides by zero
    REQUIRE(interpreter.parse(iss)); // Ensure parsing is successful
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an exception
}

// Test case for multiple define statements inside a 'begin' block
TEST_CASE("Test multiple define statements in begin", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(begin (define x 10) (define y 20) (+ x y))");
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    Expression result = interpreter.eval(); // Evaluate the expression
    REQUIRE(result == Expression(30.0)); // Expect result to be 30
}

// Test case for logical operations with non-boolean values
TEST_CASE("Test logical operations with non-boolean values", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(and 1 2 3)"); // Logical 'and' with non-boolean values
    REQUIRE(interpreter.parse(iss)); // Parsing should succeed
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an error
}

// Test case for evaluating nested mathematical expressions
TEST_CASE("Test nested expressions", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(+ (* 2 3) (/ 10 2))"); // Nested operations
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    Expression result = interpreter.eval(); // Evaluate the expression
    REQUIRE(result == Expression(11.0)); // Expected result is 11
}

// Test case for redeclaring an already defined symbol
TEST_CASE("Test redeclaring a symbol", "[interpreter]") {
    Interpreter interpreter;
    std::istringstream iss("(begin (define x 10) (define x 20))"); // Attempt to redefine 'x'
    REQUIRE(interpreter.parse(iss)); // Ensure parsing works
    REQUIRE_THROWS_AS(interpreter.eval(), InterpreterSemanticError); // Expect an error
}

// ------------------------------- Tokenization Tests -------------------------------

// Test case for tokenizing symbols that include special characters
TEST_CASE("Test tokenization of symbols with special characters", "[tokenize]") {
    std::istringstream input("(define !special_symbol 10)");
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "define", "!special_symbol", "10", ")"})); // Expect correct token sequence
}

// Test case for handling inconsistent spacing between tokens
TEST_CASE("Test tokenization with mixed spacing", "[tokenize]") {
    std::istringstream input("( +  1   2 )"); // Extra spaces between tokens
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "+", "1", "2", ")"})); // Expect correctly parsed tokens
}

// Test case for handling consecutive parentheses
TEST_CASE("Test tokenization with multiple consecutive parentheses", "[tokenize]") {
    std::istringstream input("(()())"); // Input with multiple parentheses
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "(", ")", "(", ")", ")"})); // Expect correct token sequence
}

// Test case for ignoring comments within expressions
TEST_CASE("Test tokenization with comments interspersed in input", "[tokenize]") {
    std::istringstream input("(+ 1 2 ; this is a comment\n 3)");
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "+", "1", "2", "3", ")"})); // Expect tokens without comment
}

// Test case for handling empty input
TEST_CASE("Test tokenization of empty input", "[tokenize]") {
    std::istringstream input(""); // Empty input stream
    auto tokens = tokenize(input);
    REQUIRE(tokens.empty()); // Expect empty token sequence
}

// Test case for handling unusual whitespace characters
TEST_CASE("Test tokenization with unusual whitespace characters", "[tokenize]") {
    std::istringstream input("(define\t x\n 10)"); // Includes tab and newline characters
    auto tokens = tokenize(input);
    REQUIRE((tokens == TokenSequenceType{"(", "define", "x", "10", ")"})); // Expect properly parsed tokens
}
