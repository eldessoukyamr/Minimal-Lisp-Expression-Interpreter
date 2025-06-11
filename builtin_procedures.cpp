#include "builtin_procedures.hpp"
#include <cmath>

void procNot(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 1 || params[0].type != BooleanType) {
        throw InterpreterSemanticError("not expects one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = !params[0].value.bool_value;
}

void procAnd(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("and expects at least one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = true;
    for (const auto& param : params) {
        if (param.type != BooleanType) {
            throw InterpreterSemanticError("and expects boolean arguments");
        }
        if (!param.value.bool_value) {
            output.head.value.bool_value = false;
            return;
        }
    }
}

void procOr(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("or expects at least one boolean argument");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = false;
    for (const auto& param : params) {
        if (param.type != BooleanType) {
            throw InterpreterSemanticError("or expects boolean arguments");
        }
        if (param.value.bool_value) {
            output.head.value.bool_value = true;
            return;
        }
    }
}

void procAdd(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("+ expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = 0;
    for (const auto& param : params) {
        if (param.type != NumberType) {
            throw InterpreterSemanticError("+ expects numeric arguments");
        }
        output.head.value.num_value += param.value.num_value;
    }
}

void procSubtract(const std::vector<Atom>& params, Expression& output) {
    if (params.empty() || params[0].type != NumberType) {
        throw InterpreterSemanticError("- expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = params[0].value.num_value;
    auto s = params.size();
    if (s == 1) {
        output.head.value.num_value = -output.head.value.num_value;
        return;
    }
    else if (s > 2)
    {
        throw InterpreterSemanticError("- expects at most two numeric argument");

    }
    
    output.head.value.num_value -= params[1].value.num_value;
}

void procMultiply(const std::vector<Atom>& params, Expression& output) {
    if (params.empty()) {
        throw InterpreterSemanticError("* expects at least one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = 1;
    for (const auto& param : params) {
        if (param.type != NumberType) {
            throw InterpreterSemanticError("* expects numeric arguments");
        }
        output.head.value.num_value *= param.value.num_value;
    }
}

void procDivide(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("/ expects two numeric arguments");
    }
    if (params[1].value.num_value == 0) {
        throw InterpreterSemanticError("Division by zero");
    }
    output.head.type = NumberType;
    output.head.value.num_value = params[0].value.num_value / params[1].value.num_value;
}

void procLog10(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 1 || params[0].type != NumberType) {
        throw InterpreterSemanticError("log10 expects one numeric argument");
    }
    output.head.type = NumberType;
    output.head.value.num_value = std::log10(params[0].value.num_value);
}

void procPow(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("pow expects two numeric arguments");
    }
    output.head.type = NumberType;
    output.head.value.num_value = std::pow(params[0].value.num_value, params[1].value.num_value);
}

void procLessThan(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("< expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value < params[1].value.num_value;
}

void procLessThanOrEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("<= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value <= params[1].value.num_value;
}

void procGreaterThan(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("> expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value > params[1].value.num_value;
}

void procGreaterThanOrEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError(">= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value >= params[1].value.num_value;
}

void procEqual(const std::vector<Atom>& params, Expression& output) {
    if (params.size() != 2 || params[0].type != NumberType || params[1].type != NumberType) {
        throw InterpreterSemanticError("= expects two numeric arguments");
    }
    output.head.type = BooleanType;
    output.head.value.bool_value = params[0].value.num_value == params[1].value.num_value;
}
