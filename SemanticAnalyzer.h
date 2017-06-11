//
// Semantic analyzer implementing visitor interface
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_SEMANTICANALYZER_H
#define FRACTUS_SEMANTICANALYZER_H

#include "Ast.h"
#include "Parser.h" //ParseException

/**
 * Tree visitor for semantic checks
 */
class SemanticAnalyzer : public Visitor {
public:
    using Prototypes = std::map<std::string, Scope*>;
    SemanticAnalyzer();
    ~SemanticAnalyzer();

    std::map<std::string, Scope*> *getPrototypes() const;

    void visit(const BinOpNode *n);
    void visit(const LogicalOp *n);
    void visit(const NumNode *n) {}
    void visit(const UnaryOpNode *n) {}
    void visit(const CompoundNode *n);
    void visit(const AssignNode *n);
    void visit(const IfNode *n);
    void visit(const WhileNode *n);
    void visit(const ReturnNode *n);
    void visit(const VarNode *n);
    void visit(const ProgramNode *n);
    void visit(const BlockNode *n);
    void visit(const VarDeclNode *n);
    void visit(const TypeNode *n);
    void visit(const ParamNode *n);
    void visit(const ProcDeclNode *n);
    void visit(const ProcCallNode *n);

private:
    Scope *currentScope ;
    Prototypes *prototypes;
};

#endif //FRACTUS_SEMANTICANALYZER_H
