//
// Interpreter (AST evaluator) of FraCtuS language
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_INTEPRETER_H
#define FRACTUS_INTEPRETER_H

#include <stack>
#include <algorithm>

#include "Ast.h"

class Interpreter /*: public Visitor*/ {
public:
    using Prototypes = std::map<std::string, Scope*>;

    std::map<Type, std::string> typeNames = {
            {Type::Bool, "boolean"},
            {Type::Int, "integer"},
            {Type::String, "string"},
            {Type::Fraction, "fraction"},
            {Type::Void, "void"}
    };

    Interpreter(Prototypes *prototypes, ProgramNode *ast);
    void interpret();
    Context &currContext();
    ProcDeclNode *getProcNodes(const std::string &name);
    void createNewContextFrame(const std::string &procName);
    void popContextFrame();


    bool isTruthy(const ValType &v);
    void checkNumberOperand(const ValType &operand);
    void checkNumberOperands(const ValType &left, const ValType &right);
private:
    Prototypes *scopes;
    std::vector<ProcDeclNode*> &procedures;
    ProgramNode *ast;
    Context *globalContext;
    std::stack<Context> contextStack;
};


#endif //FRACTUS_INTEPRETER_H
