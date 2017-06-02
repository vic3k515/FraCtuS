//
// Parser source file
// Wiktor Franus, WUT 2017
//

#include <iostream>

#include "Parser.h"

Parser::Parser(Scanner &scanner)
    : scanner(scanner) {
    relOp = { EQOP, LTOP, GTOP, LEOP, GEOP, NEQOP, OROP, ANDOP };
    addOp = { PLUS, MINUS };
    multOp = { MULTSIGN, DIVSIGN };
    sign = { PLUS, MINUS };
    varTypes = { STRINGTYPE, BOOLEANTYPE, INTEGERTYPE, FRACTIONTYPE };
    retTypes = { VOIDTYPE, STRINGTYPE, BOOLEANTYPE, INTEGERTYPE, FRACTIONTYPE };
    procParams = { IDENTIFIER, INTCONST, FRACTCONST, CHARCONST };

    nextSymbol();
}

void Parser::accept(const Token &tkn) {
    if (symbol == tkn) {
            nextSymbol();
    } else {
        throw ParseException("Syntax error: unexpected atom");
    }
}

void Parser::accept(const SymSet& sset) {
    if (sset.find(symbol) != sset.end()) {
            nextSymbol();
    } else {
        throw ParseException("Syntax error: unexpected atom");
    }
}

bool Parser::has(const SymSet &sset, const Token &tkn) {
    return sset.find(tkn) != sset.end();
}

void Parser::nextSymbol() {
    symbol = scanner.nextSymbol();
}

ProgramNode* Parser::parse() {
    ProgramNode *node = program();
    if (symbol != END_OF_FILE) {
        std::cout << "Parse error: current token '"<< symbol
                  << "' is different than EOF!" << std::endl;
    }
    return node;
}

ProgramNode* Parser::program() {
    /**
     *  "program" , Identifier , ';' , Block , '.'
     */
    accept(PROGRAM);
    VarNode *varNode = var();
    std::cout << "Program name: " << varNode->name << std::endl;
    accept(SEMICOLON);
    BlockNode *blockNode = block();
    ProgramNode *programNode = new ProgramNode(varNode->name, blockNode);
    accept(PERIOD);
    return programNode;
}

BlockNode* Parser::block() {
    /**
     * VarPart , FuncPart , CompStment
     */
    std::vector<VarDeclNode*>  varDecls;
    std::vector<ProcDeclNode*> procDecls;
    varDecls = variablePart();
    procDecls = functionPart();
    CompoundNode *compoundNode = compoundStatement();
    BlockNode *blockNode = new BlockNode(varDecls, procDecls, compoundNode);
    return blockNode;
}

std::vector<VarDeclNode*> Parser::variablePart() {
    /**
     * empty | "var" , VarDecl , ';' , { VarDecl , ';' }
     */
    std::vector<VarDeclNode*> varDecls;
    if (symbol == VAR) {
        accept(VAR);
        do {
            std::vector<VarDeclNode*> varDecl = variableDeclaration();
            varDecls.insert( varDecls.end(), varDecl.begin(), varDecl.end());
            accept(SEMICOLON);
        } while (symbol == IDENTIFIER);
    } else {
        // empty
    }
    return varDecls;
}

std::vector<VarDeclNode*> Parser::variableDeclaration() {
    /**
     * Identifier , { ',' , Identifier }, ':' , Type ;
     */
    std::vector<VarDeclNode*> varDecls;
    std::vector<VarNode*> varNodes;
    varNodes.push_back(var());

    while (symbol == COMMA) {
        accept(COMMA);
        varNodes.push_back(new VarNode(scanner.getLastString()));
        accept(IDENTIFIER);
    }
    accept(COLON);

    TypeNode *typeNode = varType();
    for (VarNode* varNode : varNodes) {
        varDecls.push_back(new VarDeclNode(varNode, typeNode));
    }
    return varDecls;
}

TypeNode* Parser::type(const SymSet &prefTypes) {
    std::string typeName = scanner.getLastString();
    accept(prefTypes);
    TypeNode *typeNode = new TypeNode(typeName);
    return typeNode;
}

TypeNode* Parser::varType() {
    /**
     * "void" , "fraction" , "integer" , "boolean" , "string"
     */
    return type(varTypes);
}

TypeNode* Parser::retType() {
    /**
     * "fraction" , "integer" , "boolean" , "string"
     */
    return type(retTypes);
}

std::vector<ProcDeclNode*> Parser::functionPart() {
    /**
     * { FuncDecl , ';' }
     */
    std::vector<ProcDeclNode*> procDecls;
    while (has(retTypes, symbol)) {
        ProcDeclNode *procDeclNode = functionDeclaration();
        procDecls.push_back(procDeclNode);
        accept(SEMICOLON);
    }
    return procDecls;
}

ProcDeclNode* Parser::functionDeclaration() {
    /**
     * Type , WhiteSpace , Identifier , '(' , [ Type , Identifier {, ',' ,
     *         Type, Identifier } ] , ')' , ';' , Block
     */
    TypeNode *rType = retType();
    std::string procName = scanner.getLastString();
    accept(IDENTIFIER);

    std::vector<ParamNode*> params;
    accept(PARENOPEN);
    while (has(varTypes, symbol)) {
        TypeNode *paramType = varType();
        VarNode *varNode = var();
        params.push_back(new ParamNode(varNode, paramType));
        if (symbol == COMMA) {
            accept(COMMA);
        }
    }
    accept(PARENCLOSE);
    accept(SEMICOLON);
    BlockNode *blockNode = block();
    ProcDeclNode *procDeclNode = new ProcDeclNode(procName, rType, params, blockNode);
    return procDeclNode;
}

CompoundNode* Parser::compoundStatement() {
    /**
     * "begin" , Stment , { ';' , Stment } , "end"
     */
    accept(BEGIN);
    std::vector<Node*> statements;
    statements.push_back(statement());

    while (symbol == SEMICOLON) {
        accept(SEMICOLON);
        statements.push_back(statement());
    }
//    do {
//        statement();
//        accept(SEMICOLON);
//    }
//    while (has({RETURN,IF,WHILE,BEGIN,IDENTIFIER},symbol));
    accept(END);
    CompoundNode *compoundNode = new CompoundNode();
    compoundNode->children = std::move(statements);
    return compoundNode;
}

Node* Parser::statement() {
    /**
     * Assignment | FuncCall | Return | IfStment | WhileStment | CompStment
     */
    Node *node;
    switch (symbol) {
        case RETURN:
            node = functionReturn();
            break;
        case IF:
            node = ifStatement();
            break;
        case WHILE:
            node = whileStatement();
            break;
        case BEGIN:
            node = compoundStatement();
            break;
        case IDENTIFIER:
            // can be assignment or function call
            VarNode *pre = var();
            //isSymbolPreloaded = true;
            if (symbol == EQALSIGN) {
                node = assignment(pre);
                break;
            } else if (symbol == PARENOPEN) {
                node = functionCall(pre);
                break;
            } else {
                throw ParseException("Syntax error: unexpected symbol");
            }
    }
    return node;
}

AssignNode* Parser::assignment(VarNode *preloadedLeft) {
    /**
     * Var , '=' , Expr
     */
    VarNode *left = preloadedLeft ? preloadedLeft : var();
    accept(EQALSIGN);
    Node *right = expression();
    AssignNode *assignNode = new AssignNode(left, right);
    return assignNode;
}

ProcCallNode* Parser::functionCall(VarNode *preloadedProcId) {
    /**
     * Identifier , '(' [, Expr, { "," , Expr } ], ')'
     */
    ProcCallNode *node;
    std::vector<Node*> args;
    VarNode *procId = preloadedProcId ? preloadedProcId : var();
    node = new ProcCallNode(procId, args);

    accept(PARENOPEN);
    if (symbol == PARENCLOSE) { // 0 arguments
        accept(PARENCLOSE);
        return node;
    }
    while (has(procParams, symbol)) {
        node->arguments.push_back(expression());
        if (symbol == COMMA) {
            accept(COMMA);
        }
    }
    accept(PARENCLOSE);
    return node;
}

Node* Parser::functionReturn() {
    /**
     * "return" , Expr
     */
    accept(RETURN);
    return new ReturnNode(expression());
}

IfNode* Parser::ifStatement() {
    /**
     * SimpleIf [, "else" , Stment ]
     */
    IfNode *node = simpleIfStatement();
    if (symbol == ELSE) {
        accept(ELSE);
        node->elseNode = statement();
    }
    return node;
}

IfNode* Parser::simpleIfStatement() {
    /**
     * "if" , Expr , "then" , Stment
     */
    Node *expr, *thenNode;
    accept(IF);
    accept(PARENOPEN);
    expr = expression();
    accept(PARENCLOSE);
    accept(THEN);
    thenNode = statement();
    return new IfNode(expr, thenNode, nullptr);
}

WhileNode* Parser::whileStatement() {
    /**
     * "while" , Expr , "do" , Stment
     */
    Node *expr, *stmt;
    accept(WHILE);
    expr = expression();
    accept(DO);
    stmt = statement();
    return new WhileNode(expr, stmt);
}

Node* Parser::expression() {
    /**
     * SimpExpr [, RelOp , SimpExpr ] ;
     */
    Node *node = simpleExpression();
    if (has(relOp, symbol)) {
        Token op = symbol;
        accept(relOp);
        node = new BinOpNode(node, op, simpleExpression());
    }
    return node;
}

Node* Parser::simpleExpression() {
    /**
     * [Sign ,] Term , { AddOp , Term } | Bool
     */
    Node *node;
    std::string id = scanner.getLastString();
    if (symbol == IDENTIFIER && (id == "true" || id == "false")) {
        node = new VarNode(id);
        accept(IDENTIFIER);
        return node;
    }

    if (has(sign, symbol)) {
        Token sign_ = symbol;
        accept(sign);
        node = new UnaryOpNode(sign_, term());
    } else {
        node = term();
    }
    while (has(addOp, symbol)) {
        Token op = symbol;
        accept(addOp);
        node = new BinOpNode(node, op, term());
    }
    return node;
}

Node* Parser::term() {
    /**
     * Factor , { MultOp , Factor }
     */
    Node *node = factor();
    while (has(multOp, symbol)) {
        Token op = symbol;
        accept(multOp);
        node = new BinOpNode(node, op, factor());
    }
    return node;
}

Node* Parser::factor() {
    /**
     * FuncCall | Var | Constant | '(' , Expr , ')' | '!' , Factor
     */
    Node *node;
    switch (symbol) {
        case IDENTIFIER: {
            // can be variable or function call
            VarNode *pre = var();
            if (symbol == PARENOPEN) {
                node = functionCall(pre);
                break;
            } else {
                node = pre;
                break;
            }
        }
        case FRACTCONST:
            node = new FractNode(symbol, scanner.getLastFraction());
            accept(FRACTCONST);
            break;
        case INTCONST:
            node = new IntNode(symbol, scanner.getLastNumber());
            accept(INTCONST);
            break;
        case CHARCONST:
            node = new StringNode(symbol, scanner.getLastString());
            accept(CHARCONST);
            break;
        case PARENOPEN:
            accept(PARENOPEN);
            node = expression();
            accept(PARENCLOSE);
            break;
        case NOTSIGN:
            accept(NOTSIGN);
            node = factor(); // TODO: treat '!' somehow
            break;
    }
    return node;
}

VarNode* Parser::var() {
    /**
     * Identifier
     */
    VarNode* varNode = new VarNode(scanner.getLastString());
    accept(IDENTIFIER);
    return varNode;
}