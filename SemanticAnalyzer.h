//
// Semantic analyzer implementing visitor interface
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_SEMANTICANALYZER_H
#define FRACTUS_SEMANTICANALYZER_H

#include "Ast.h"
#include "Scope.h"
#include "Parser.h" //ParseException

class SemanticAnalyzer : public Visitor {
public:
    SemanticAnalyzer();
    ~SemanticAnalyzer() {};

    Scope* currentScope ;

    void visit(const BinOpNode *n);
    void visit(const NumNode *n) {}
    void visit(const UnaryOpNode *n) {}
    void visit(const CompoundNode *n);
    void visit(const AssignNode *n);
    void visit(const IfNode *n);
    void visit(const WhileNode *n);
    void visit(const ReturnNode *n);
    void visit(const VarNode *n);
    //void visit(const NoOpNode *n) {}
    void visit(const ProgramNode *n);
    void visit(const BlockNode *n);
    void visit(const VarDeclNode *n);
    void visit(const TypeNode *n);
    void visit(const ParamNode *n);
    void visit(const ProcDeclNode *n);
    void visit(const ProcCallNode *n);
};

#endif //FRACTUS_SEMANTICANALYZER_H
