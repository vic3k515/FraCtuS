//
// Interpreter source file
// Wiktor Franus, WUT 2017
//

#include "Interpreter.h"

Interpreter::Interpreter(Prototypes *prototypes, ProgramNode *ast)
: scopes(prototypes)
, procedures(ast->block->procDeclarations)
, ast(ast)
, globalContext(nullptr)
{}

void Interpreter::interpret() {
    if (scopes && ast) {
        createNewContextFrame("global");
        try {
            ast->evaluate(this);
        } catch (std::runtime_error e) {
            std::cout<< "Runtime error: " << e.what() << std::endl;
        }
    }
}

void Interpreter::createNewContextFrame(const std::string &procName) {
    Scope *procScope = nullptr;
    try {
        procScope = scopes->at(procName);
    } catch (std::out_of_range e) {
        throw std::runtime_error("Cannot find declared procedure: " + procName);
    }

    if (!globalContext) {
        Context runtimeContext(procScope, nullptr); // variable initialization inside Context c-tor
        contextStack.push(std::move(runtimeContext));
        globalContext = &contextStack.top();

    } else {
        Context runtimeContext(procScope, globalContext); // variable initialization inside Context c-tor
        contextStack.push(std::move(runtimeContext));
    }
}

void Interpreter::popContextFrame() {
    contextStack.pop();
}



void Interpreter::checkNumberOperand(const ValType &operand) {
    if (operand.second == Type::Int || operand.second == Type::Fraction) {
        return;
    }
    throw std::runtime_error("Operand must be a number.");
}

void Interpreter::checkNumberOperands(const ValType &left, const ValType &right) {
    if ((left.second == Type::Int || left.second == Type::Fraction)
        && (right.second == Type::Int || right.second == Type::Fraction)) {
        return;
    }
    throw std::runtime_error("Operands must be numbers.");
}

void Interpreter::checkDifferentThanZero(const ValType &operand) {
    if ((operand.second == Type::Int && operand.first.intVal != 0)
        || (operand.second == Type::Fraction || operand.first.fractVal.numerator != 0)) {
        return;
    }
    throw std::runtime_error("Operand must be different than 0.");
}

Context &Interpreter::currContext() {
    return contextStack.top();
}

ProcDeclNode *Interpreter::getProcNodes(const std::string &name) {
    auto it = std::find_if(procedures.begin(), procedures.end(),
                           [&](const ProcDeclNode *p) { return p->name == name; }
    );
    if (it != procedures.end()) {
        return *it;
    }
    return nullptr;
}

bool Interpreter::isTruthy(const ValType &v) {
    if (v.second == Type::Void) return false;
    if (v.second == Type::Bool) return v.first.boolVal;
    return true;
}

