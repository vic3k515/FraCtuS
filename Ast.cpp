//
// Created by franek on 01.06.17.
//

#include "Ast.h"

void BinOpNode::accept(Visitor &v) const {
    v.visit(this);
}

void IntNode::accept(Visitor &v) const {
    v.visit(this);
}

void FractNode::accept(Visitor &v) const {
    v.visit(this);
}

void BoolNode::accept(Visitor &v) const {
    v.visit(this);
}

void StringNode::accept(Visitor &v) const {
    v.visit(this);
}

void UnaryOpNode::accept(Visitor &v) const {
    v.visit(this);
}

void CompoundNode::accept(Visitor &v) const {
    v.visit(this);
}

void AssignNode::accept(Visitor &v) const {
    v.visit(this);
}

void IfNode::accept(Visitor &v) const {
    v.visit(this);
}

void WhileNode::accept(Visitor &v) const {
    v.visit(this);
}

void ProcCallNode::accept(Visitor &v) const {
    v.visit(this);
}

void ReturnNode::accept(Visitor &v) const {
    v.visit(this);
}

void VarNode::accept(Visitor &v) const {
    v.visit(this);
}

//void NoOpNode::accept(Visitor &v) const {
//    v.visit(this);
//}

void ProgramNode::accept(Visitor &v) const {
    v.visit(this);
}

void BlockNode::accept(Visitor &v) const {
    v.visit(this);
}

void VarDeclNode::accept(Visitor &v) const {
    v.visit(this);
}

void TypeNode::accept(Visitor &v) const {
    v.visit(this);
}

void ProcDeclNode::accept(Visitor &v) const {
    v.visit(this);
}

void ParamNode::accept(Visitor &v) const {
    v.visit(this);
}