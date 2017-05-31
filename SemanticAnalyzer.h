//
// Created by franek on 31.05.17.
//

#ifndef FRACTUS_SEMANTICANALYZER_H
#define FRACTUS_SEMANTICANALYZER_H

#include "Scope.h"

class Visitor;

struct Node {
    virtual ~Node() {}
    virtual void accept(Visitor &v) const = 0;
};


class Visitor {
public:
//    virtual const void visit(const Htmlnode *n) = 0;
//    virtual const void visit(const Emptyhtmlnode *n) = 0;
//    virtual const void visit(const Textnode *n) = 0;
};


class SemanticAnalyzer : public Visitor {
public:
    SemanticAnalyzer() {};
    ~SemanticAnalyzer() {};

    Scope* currentScope = nullptr;
private:
    void visitBlock();
    void visitProgram();
    void visitCompund();
    void visitNoOp();
    void visitProcDecl();
    void visitVarDecl();
    void visitAssign();
    void visitVar();
};


#endif //FRACTUS_SEMANTICANALYZER_H

