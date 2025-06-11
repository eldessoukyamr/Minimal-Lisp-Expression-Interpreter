#ifndef TYPES_HPP
#define TYPES_HPP

// system includes
#include <string>
#include <vector>

// Enumeration of possible expression types
enum Type {NoneType, BooleanType, NumberType, ListType, SymbolType};

// Define Boolean as an alias for bool
typedef bool Boolean;

// Define Number as an alias for double
typedef double Number;

// Define Symbol as an alias for std::string
typedef std::string Symbol;

// Structure to hold the value of an Atom
struct Value {
  Boolean bool_value;  // Stores Boolean values
  Number num_value;    // Stores numeric values
  Symbol sym_value;    // Stores symbol values
};

// Structure representing an Atom
struct Atom {
  Type type;  // Type of the atom
  Value value; // Value of the atom
};

// Structure representing an Expression
struct Expression {
  Atom head;                  // Head of the expression (an Atom)
  std::vector<Expression> tail; // Tail containing sub-expressions

  // Default constructor initializes the head to NoneType
  Expression() {
    head.type = NoneType;
  };

  // Constructor for initializing from an Atom
  Expression(const Atom & atom) : head(atom) {};

  // Constructor for BooleanType expression
  Expression(bool tf);

  // Constructor for NumberType expression
  Expression(double num);

  // Constructor for SymbolType expression
  Expression(const std::string & sym);

  // Equality operator for comparing two expressions
  bool operator==(const Expression & exp) const noexcept;
};

// Procedure type: function pointer that takes a vector of Atoms and returns an Expression
typedef Expression (*Procedure)(const std::vector<Atom> & args);

// Overloaded operator<< to print an Expression
std::ostream & operator<<(std::ostream & out, const Expression & exp);

// Converts a token to an Atom
bool token_to_atom(const std::string & token, Atom & atom);

#endif
