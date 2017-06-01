//
// Parser for FraCtuS language
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_PARSER_H
#define FRACTUS_PARSER_H

#include <set>

#include "Scope.h"
#include "Ast.h"

class ParseException : public std::runtime_error {
public:
    ParseException(std::string msg) : runtime_error(msg) {};

//    const char* what() const throw() {
//      return err.c_str();
//    }
//
//  private:
//    std::string err;
};

//class Scope;

class Parser {
public:
    using SymSet = std::set<Token>;

    Parser(Scanner &scanner);
    ProgramNode* parse();
    Token getCurrSymbol() {
        return symbol;
    }

private:
    void accept(const Token& tkn);
    void accept(const SymSet& sset);
    bool has(const SymSet &sset, const Token &tkn);
    //void openScope(const std::string &scopeName);
    void nextSymbol();
    ProgramNode* program();
    BlockNode* block();
    std::vector<VarDeclNode*> variablePart();
    std::vector<VarDeclNode*> variableDeclaration();
    TypeNode* retType();
    TypeNode* varType();
    TypeNode* type(const SymSet &prefTypes);
    std::vector<ProcDeclNode*> functionPart();
    ProcDeclNode* functionDeclaration();
    CompoundNode* compoundStatement();
    Node* statement();
    AssignNode* assignment();
    ProcCallNode* functionCall();
    Node* functionReturn();
    IfNode* ifStatement();
    IfNode* simpleIfStatement();
    WhileNode* whileStatement();
    Node* expression();
    Node* simpleExpression();
    Node* term();
    Node* factor();
    VarNode* var();

    SymSet  relOp,
            addOp,
            multOp,
            sign,
            varTypes,
            retTypes;

    Scanner &scanner;
    Scope *scope;
    Token symbol;
};

#endif //FRACTUS_PARSER_H
