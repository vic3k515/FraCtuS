//
// Created by franek on 01.06.17.
//

#ifndef FRACTUS_AST_H
#define FRACTUS_AST_H

#include <vector>

#include "Scanner.h"

class Visitor;

// forward deckarations
struct VarDeclNode;
struct ProcDeclNode;
struct BlockNode;
struct TypeNode;
struct VarNode;

struct Node {
    virtual ~Node() {}
    virtual void accept(Visitor &v) const = 0;
};

struct BinOpNode : public Node {
    BinOpNode(Node *l, Token op, Node *r) : left(l), op(op), right(r) {}
    void accept(Visitor &v) const;

    Node *left;
    Token op;
    Node *right;
};

struct NumNode : public Node {
    NumNode(Token t) : token(t) {}
    void accept(Visitor &v) {}

    Token token;
};

struct IntNode : public NumNode {
    IntNode(Token t, int v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;

    int value;
};

struct FractNode : public NumNode {
    FractNode(Token t, Fraction v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;

    Fraction value;
};

struct BoolNode : public NumNode {
    BoolNode(Token t, bool v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;

    bool value;
};

struct StringNode : public NumNode {
    StringNode(Token t, const std::string &v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;

    std::string value;
};

struct UnaryOpNode : public Node {
    UnaryOpNode(Token op, Node* ex) : op(op), expression(ex) {}
    void accept(Visitor &v) const;

    Token op;
    Node *expression;
};

struct CompoundNode : public Node {
    CompoundNode() {}
    void accept(Visitor &v) const;

    std::vector<Node*> children;
};

struct AssignNode : public Node {
    AssignNode(VarNode *l, Node *r) : left(l), right(r) {}
    void accept(Visitor &v) const;

    VarNode *left;
    Node *right;
};

struct IfNode : public Node {
    IfNode(Node *c, Node *t, Node *e)
            : condition(c), thenNode(t), elseNode(e) {}
    void accept(Visitor &v) const;

    Node *condition;
    Node *thenNode;
    Node *elseNode;
};

struct WhileNode : public Node {
    WhileNode(Node *c, Node *s)
            : condition(c), statement(s) {}
    void accept(Visitor &v) const;

    Node *condition;
    Node *statement;
};

struct ReturnNode : public Node {
    ReturnNode(Node *e) : expr(e) {}
    void accept(Visitor &v) const;

    Node *expr;
};

struct VarNode : public Node {
    VarNode(const std::string &n) : name(n) {}
    void accept(Visitor &v) const;

    //Token token;
    std::string name;
};

//struct NoOpNode : public Node {
//    NoOpNode() {}
//    void accept(Visitor &v) const;
//};

struct ProgramNode : public Node {
    ProgramNode(const std::string &n, BlockNode* b) : name(n), block(b) {}
    void accept(Visitor &v) const;

    std::string name;
    BlockNode *block;
};

struct BlockNode : public Node {
    BlockNode(std::vector<VarDeclNode*> &vd, std::vector<ProcDeclNode*> &pd, CompoundNode* cS)
            : varDeclarations(vd), procDeclarations(pd), compundStatement(cS) {}
    void accept(Visitor &v) const;

    std::vector<VarDeclNode*> varDeclarations;
    std::vector<ProcDeclNode*> procDeclarations;
    CompoundNode *compundStatement;
};

struct VarDeclNode : public Node {
    VarDeclNode(VarNode* vn, TypeNode* tn) : varNode(vn), typeNode(tn) {}
    void accept(Visitor &v) const;

    VarNode *varNode;
    TypeNode *typeNode;
};

struct TypeNode : public Node {
    TypeNode(const std::string &tn) : typeName(tn) {}
    void accept(Visitor &v) const;

    std::string typeName;
};

struct ParamNode : public Node {
    ParamNode(VarNode* vn, TypeNode* tn) : varNode(vn), typeNode(tn) {}
    void accept(Visitor &v) const;

    VarNode *varNode;
    TypeNode *typeNode;
};

struct ProcDeclNode : public Node {
    ProcDeclNode(const std::string &n, TypeNode* rt, std::vector<ParamNode*> &p, BlockNode* b)
            : name(n), returnType(rt), params(p), blockNode(b) {}
    void accept(Visitor &v) const;

    std::string name;
    TypeNode* returnType;
    std::vector<ParamNode*> params; // vec of ParamNodes
    BlockNode *blockNode;
};

struct ProcCallNode : public Node {
    ProcCallNode(VarNode *vn, std::vector<Node*> a) : proc(vn), arguments(a) {}
    void accept(Visitor &v) const;

    VarNode *proc;
    std::vector<Node*> arguments;
};

class Visitor {
public:
    virtual void visit(const BinOpNode *n) = 0;
    virtual void visit(const NumNode *n) = 0;
    virtual void visit(const UnaryOpNode *n) = 0;
    virtual void visit(const CompoundNode *n) = 0;
    virtual void visit(const AssignNode *n) = 0;
    virtual void visit(const IfNode *n) = 0;
    virtual void visit(const WhileNode *n) = 0;
    virtual void visit(const ReturnNode *n) = 0;
    virtual void visit(const VarNode *n) = 0;
    //virtual void visit(const NoOpNode *n) = 0;
    virtual void visit(const ProgramNode *n) = 0;
    virtual void visit(const BlockNode *n) = 0;
    virtual void visit(const VarDeclNode *n) = 0;
    virtual void visit(const TypeNode *n) = 0;
    virtual void visit(const ParamNode *n) = 0;
    virtual void visit(const ProcDeclNode *n) = 0;
    virtual void visit(const ProcCallNode *n) = 0;
};


#endif //FRACTUS_AST_H
