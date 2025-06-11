#include "catch.hpp"

#include <string>

#include "expression.hpp"

// Test type inference of tokens to Atom
TEST_CASE( "Test Type Inference", "[types]" ) {

  Atom a;
  
  // Boolean True
  std::string token = "True";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == true);

  // Boolean False
  token = "False";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == BooleanType);
  REQUIRE(a.value.bool_value == false);

  // Positive Number
  token = "1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == 1);

  // Negative Number
  token = "-1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == NumberType);
  REQUIRE(a.value.num_value == -1);

  // Symbol
  token = "var";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == "var");

  // Invalid token (bad number string)
  token = "1abc";
  REQUIRE(!token_to_atom(token, a));

  // Symbol with numbers
  token = "var1";
  REQUIRE(token_to_atom(token, a));
  REQUIRE(a.type == SymbolType);
  REQUIRE(a.value.sym_value == token);
}

// Test Expression constructors
TEST_CASE( "Test Expression Constructors", "[types]" ) {

  // Default constructor should create an empty Expression
  Expression exp1;
  REQUIRE(exp1 == Expression());
}
