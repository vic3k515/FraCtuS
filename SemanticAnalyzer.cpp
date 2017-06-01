//
// Created by franek on 31.05.17.
//

#include "SemanticAnalyzer.h"

SemanticAnalyzer::SemanticAnalyzer()
: currentScope(nullptr)
{}

void SemanticAnalyzer::visit(const BlockNode *n) {
    for (auto node : n->varDeclarations) {
        //visit(node);
        node->accept(*this);
    }
    for (auto node : n->procDeclarations) {
//        visit(node);
        node->accept(*this);
    }
    n->compundStatement->accept(*this);
    //visit(n->compundStatement);
}

void SemanticAnalyzer::visit(const ProgramNode *n) {
    std::cout << "ENTER scope: global" << std::endl;
    Scope* global_scope = new Scope("global", 1, currentScope);
    global_scope->initializeBuiltInTypes();

    currentScope = global_scope;

    visit(n->block);

    std::cout << *global_scope << std::endl;

    currentScope = currentScope->getEnclosingScope();
    std::cout << "LEAVE scope: global" << std::endl;
}

void SemanticAnalyzer::visit(const CompoundNode *n) {
    for (auto child : n->children) {
//        visit(child);
        child->accept(*this);
    }
}

void SemanticAnalyzer::visit(const AssignNode *n) {
    n->left->accept(*this);
    n->right->accept(*this);
}

void SemanticAnalyzer::visit(const BinOpNode *n) {
//    visit(n->left);
//    visit(n->right);
    n->left->accept(*this);
    n->right->accept(*this);
}

void SemanticAnalyzer::visit(const ProcDeclNode *n) {
    std::string procName = n->name;
    std::string retType = n->returnType->typeName;
    std::vector<VarDescriptor*> params;
    ProcDescriptor* procDesc = new ProcDescriptor(procName, retType, params);
    currentScope->insert(procDesc);

    std::cout << "ENTER scope: " << procName << std::endl;
    Scope* procScope = new Scope(procName, currentScope->getLevel() + 1, currentScope);
    currentScope = procScope;

    for (ParamNode* param : n->params) {
        Descriptor* descriptor = currentScope->lookup(param->typeNode->typeName);
        BuiltInTypeDescriptor* typeDescriptor = static_cast<BuiltInTypeDescriptor*>(descriptor);
        VarDescriptor* varDesc = new VarDescriptor(param->varNode->name, typeDescriptor);
        currentScope->insert(varDesc);
        procDesc->params.push_back(varDesc);
    }

    //visit(n->blockNode);
    n->blockNode->accept(*this);

    std::cout << *procScope << std::endl;

    currentScope = currentScope->getEnclosingScope();
    std::cout << "LEAVE scope: " << procName << std::endl;
}

void SemanticAnalyzer::visit(const IfNode *n) {

}

void SemanticAnalyzer::visit(const WhileNode *n) {

}

void SemanticAnalyzer::visit(const ReturnNode *n) {

}

void SemanticAnalyzer::visit(const VarNode *n) {
    if (currentScope->lookup(n->name) == nullptr) {
        throw ParseException("Semantic error: Symbol(identifier) not found '" + n->name + "'");
    }
}

void SemanticAnalyzer::visit(const VarDeclNode *n) {
    std::string typeName = n->typeNode->typeName;
    Descriptor* descriptor = currentScope->lookup(typeName);
    BuiltInTypeDescriptor* typeDescriptor = static_cast<BuiltInTypeDescriptor*>(descriptor);

    std::string varName = n->varNode->name;
    VarDescriptor* varDescriptor = new VarDescriptor(varName, typeDescriptor);

    if (currentScope->lookup(varName, true) != nullptr) {
        throw ParseException("Semantic error: Duplicate identifier " + varName + " found");
    }

    currentScope->insert(varDescriptor);
}

void SemanticAnalyzer::visit(const TypeNode *n) {

}

void SemanticAnalyzer::visit(const ParamNode *n) {

}

void SemanticAnalyzer::visit(const ProcCallNode *n) {

}
