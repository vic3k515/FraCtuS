//
// Parser for FraCtuS language
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_PARSER_H
#define FRACTUS_PARSER_H

#include <set>

#include "Ast.h"

class ParseException : public std::runtime_error {
public:
    ParseException(std::string msg) : runtime_error(msg) {};
};


/**
 * Parser of FraCtuS language
 */
class Parser {
    using SymSet = std::set<Token>;

public:
    Parser(Scanner &scanner);
    ~Parser() {}

    ProgramNode* parse();
    Token getCurrSymbol() const {
        return symbol;
    }

private:
    void accept(const Token& tkn);
    void accept(const SymSet& sset);
    bool has(const SymSet &sset, const Token &tkn);
    void nextSymbol();

    // RD parser methods (for each non-terminal in grammar):
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
    AssignNode* assignment(VarNode *left = nullptr);
    ProcCallNode* functionCall(VarNode *procId = nullptr);
    Node* functionReturn();
    IfNode* ifStatement();
    IfNode* simpleIfStatement();
    WhileNode* whileStatement();
    Node* expression();
    Node* simpleExpression();
    Node* term();
    Node* factor();
    VarNode* var();

    // subsets of tokens
    SymSet  relOp,
            addOp,
            multOp,
            sign,
            varTypes,
            retTypes,
            procParams;

    Scanner &scanner;
    Token symbol;
};

#endif //FRACTUS_PARSER_H
