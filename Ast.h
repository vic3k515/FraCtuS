//
// Representation of AST nodes using visitor pattern
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_AST_H
#define FRACTUS_AST_H

#include <vector>
#include <memory>

#include "Scanner.h"
#include "Scope.h"

// forward declarations
class Visitor;
class Interpreter;

struct VarDeclNode;
struct ProcDeclNode;
struct BlockNode;
struct TypeNode;
struct VarNode;

/**
 * AST nodes classes
 */
struct Node {
    virtual ~Node() {}
    virtual void accept(Visitor &v) const = 0;
    virtual ValType evaluate(Interpreter *interpreter) = 0;
};

struct NumNode : public Node {
    NumNode(Token t) : token(t) {}
    void accept(Visitor &v) {}
    virtual ValType evaluate(Interpreter *interpreter) {};

    Token token;
};

struct IntNode : public NumNode {
    IntNode(Token t, int v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    int value;
};

struct FractNode : public NumNode {
    FractNode(Token t, Fraction v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Fraction value;
};

struct BoolNode : public NumNode {
    BoolNode(Token t, bool v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    bool value;
};

struct StringNode : public NumNode {
    StringNode(Token t, const std::string &v) : NumNode(t), value(v) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::string value;
};

struct BinOpNode : public Node {
    BinOpNode(Node *l, Token op, Node *r) : left(l), op(op), right(r) {}
    ~BinOpNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Node *left;
    Token op;
    Node *right;
};

struct LogicalOp : public BinOpNode {
    LogicalOp(Node *l, Token op, Node *r) : BinOpNode(l, op, r) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);
};

struct UnaryOpNode : public Node {
    UnaryOpNode(Token op, Node* ex) : op(op), expression(ex) {}
    ~UnaryOpNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Token op;
    Node *expression;
};

struct CompoundNode : public Node {
    CompoundNode() {}
    ~CompoundNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::vector<Node*> children;
};

struct AssignNode : public Node {
    AssignNode(VarNode *l, Node *r) : left(l), right(r) {}
    ~AssignNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    VarNode *left;
    Node *right;
};

struct IfNode : public Node {
    IfNode(Node *c, Node *t, Node *e)
            : condition(c), thenNode(t), elseNode(e) {}
    ~IfNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Node *condition;
    Node *thenNode;
    Node *elseNode;
};

struct WhileNode : public Node {
    WhileNode(Node *c, Node *s)
            : condition(c), statement(s) {}
    ~WhileNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Node *condition;
    Node *statement;
};

struct ReturnNode : public Node {
    ReturnNode(Node *e) : expr(e) {}
    ~ReturnNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    Node *expr;
};

struct VarNode : public Node {
    VarNode(const std::string &n) : name(n) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::string name;
};

struct ProgramNode : public Node {
    ProgramNode(const std::string &n, BlockNode* b) : name(n), block(b) {}
    ~ProgramNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::string name;
    BlockNode *block;
};

struct BlockNode : public Node {
    BlockNode(std::vector<VarDeclNode*> &vd, std::vector<ProcDeclNode*> &pd, CompoundNode* cS)
            : varDeclarations(vd), procDeclarations(pd), compundStatement(cS) {}
    ~BlockNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::vector<VarDeclNode*> varDeclarations;
    std::vector<ProcDeclNode*> procDeclarations;
    CompoundNode *compundStatement;
};

struct VarDeclNode : public Node {
    VarDeclNode(VarNode* vn, const std::shared_ptr<TypeNode> &tn) : varNode(vn), typeNode(tn) {}
    ~VarDeclNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter) {}

    VarNode *varNode;
    std::shared_ptr<TypeNode> typeNode; //multiple VarDeclNodes use the same TypeNode object
    //TypeNode *typeNode;
};

struct TypeNode : public Node {
    TypeNode(const std::string &tn) : typeName(tn) {}
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    std::string typeName;
};

struct ParamNode : public Node {
    ParamNode(VarNode* vn, const std::shared_ptr<TypeNode> &tn) : varNode(vn), typeNode(tn) {}
    ~ParamNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    VarNode *varNode;
    std::shared_ptr<TypeNode> typeNode;
    //TypeNode *typeNode;
};

struct ProcDeclNode : public Node {
    ProcDeclNode(const std::string &n, TypeNode* rt, std::vector<ParamNode*> &p, BlockNode* b)
            : name(n), returnType(rt), params(p), blockNode(b) {}
    ~ProcDeclNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter) {}

    std::string name;
    TypeNode* returnType;
    std::vector<ParamNode*> params; // vec of ParamNodes
    BlockNode *blockNode;
};

struct ProcCallNode : public Node {
    ProcCallNode(VarNode *vn, std::vector<Node*> a) : proc(vn), arguments(a) {}
    ~ProcCallNode();
    void accept(Visitor &v) const;
    ValType evaluate(Interpreter *interpreter);

    VarNode *proc;
    std::vector<Node*> arguments;
};

/**
 * AST visitor interface
 */
class Visitor {
public:
    virtual void visit(const BinOpNode *n) = 0;
    virtual void visit(const LogicalOp *n) = 0;
    virtual void visit(const NumNode *n) = 0;
    virtual void visit(const UnaryOpNode *n) = 0;
    virtual void visit(const CompoundNode *n) = 0;
    virtual void visit(const AssignNode *n) = 0;
    virtual void visit(const IfNode *n) = 0;
    virtual void visit(const WhileNode *n) = 0;
    virtual void visit(const ReturnNode *n) = 0;
    virtual void visit(const VarNode *n) = 0;
    virtual void visit(const ProgramNode *n) = 0;
    virtual void visit(const BlockNode *n) = 0;
    virtual void visit(const VarDeclNode *n) = 0;
    virtual void visit(const TypeNode *n) = 0;
    virtual void visit(const ParamNode *n) = 0;
    virtual void visit(const ProcDeclNode *n) = 0;
    virtual void visit(const ProcCallNode *n) = 0;
};


#endif //FRACTUS_AST_H
