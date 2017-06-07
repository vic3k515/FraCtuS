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

//    void visit(const BinOpNode *n);
//    void visit(const NumNode *n) {}
//    void visit(const UnaryOpNode *n) {}
//    void visit(const CompoundNode *n);
//    void visit(const AssignNode *n);
//    void visit(const IfNode *n);
//    void visit(const WhileNode *n);
//    void visit(const ReturnNode *n);
//    void visit(const VarNode *n);
//    //void visit(const NoOpNode *n) {}
//    void visit(const ProgramNode *n);
//    void visit(const BlockNode *n);
//    void visit(const VarDeclNode *n);
//    void visit(const TypeNode *n);
//    void visit(const ParamNode *n);
//    void visit(const ProcDeclNode *n);
//    void visit(const ProcCallNode *n);

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
