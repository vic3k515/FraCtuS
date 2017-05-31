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

    openScope("global");
    scope->insert("integer", Int);
    scope->insert("string", String);
    scope->insert("fraction", Fraction);
    scope->insert("false", Bool);
    scope->insert("true", Bool);
    scope->insert("program", Proc);

    nextSymbol();
}

void Parser::accept(const Token &tkn) {
    if (symbol == tkn) {
        nextSymbol();
    } else {
        throw ParseException("unexpected atom");
    }
}

void Parser::accept(const SymSet& sset) {
    if (sset.find(symbol) != sset.end()) {
        nextSymbol();
    } else {
        throw ParseException("unexpected atom");
    }
}

bool Parser::has(const SymSet &sset, const Token &tkn) {
    return sset.find(tkn) != sset.end();
}

void Parser::openScope(const std::string &scopeName) {
    scope = new Scope(scopeName, *this, nullptr);
}

void Parser::nextSymbol() {
    symbol = scanner.nextSymbol();
}

void Parser::parse() {
    accept(PROGRAM);

    if(symbol == IDENTIFIER) {
        std::cout << "Program name: " << scanner.getLastString() << std::endl;
        accept(IDENTIFIER);
    }
    accept(SEMICOLON);
    block();
    accept(PERIOD);
}

void Parser::block() {
    variablePart();
    functionPart();
    compoundStatement();
}

void Parser::variablePart() {
    if (symbol == VAR) {
        accept(VAR);
        do {
            variableDeclaration();
            accept(SEMICOLON);
        } while (symbol == IDENTIFIER);
    }
}

void Parser::variableDeclaration() {
    accept(IDENTIFIER);
    while (symbol == COMMA) {
        accept(COMMA);
        // install var here
        accept(IDENTIFIER);
    }
    accept(COLON);
    type();
}

void Parser::type() {
//    // check type here
//    if (symbol == IDENTIFIER) {
//        std::string s = scanner.getLastString();
//        if (s == "fraction" ||
//            s == "integer" ||
//            s == "boolean" ||
//            s == "string") {
//            accept(IDENTIFIER);
//            return;
//        }
//    }
//    throw ParseException("unexpected variable type");
    accept(varTypes);
}

void Parser::functionPart() {
    while (has(retTypes, symbol)) {
        functionDeclaration();
        accept(SEMICOLON);
    }
}

void Parser::functionDeclaration() {
    // install function here
    accept(retTypes); // return type
    accept(IDENTIFIER); // function name
    accept(PARENOPEN);
    while (has(varTypes, symbol)) {
        // read function arguments
        accept(varTypes); // arg type
        accept(IDENTIFIER); // arg name
        if (symbol == COMMA) {
            accept(COMMA);
        }
    }
    accept(PARENCLOSE);
    accept(SEMICOLON);
    block();
}

void Parser::compoundStatement() {
    accept(BEGIN);
    statement();
    while (symbol == SEMICOLON) {
        accept(SEMICOLON);
        statement();
    }
//    do {
//        statement();
//        accept(SEMICOLON);
//    }
//    while (has({RETURN,IF,WHILE,BEGIN,IDENTIFIER},symbol));
    accept(END);
}

void Parser::statement() {
    switch (symbol) {
        case RETURN:
            functionReturn();
            break;
        case IF:
            ifStatement();
            break;
        case WHILE:
            whileStatement();
            break;
        case BEGIN:
            compoundStatement();
            break;
        case IDENTIFIER:
            // can be assignment or function call
            if (symbol) { // FIXME: if symbol is already declared variable identifier
                assignment();
            } else {
                functionCall();
            }
            break;
    }
}

void Parser::assignment() {
    var();
    accept(EQALSIGN);
    expression();
}

void Parser::functionCall() {
    accept(IDENTIFIER);
    accept(PARENOPEN);
    if (symbol == PARENCLOSE) { // 0 parameters
        accept(PARENCLOSE);
        return;
    }
    while (symbol == IDENTIFIER) {
        expression();
        if (symbol == COMMA) {
            accept(COMMA);
        }
    }
    accept(PARENCLOSE);
}

void Parser::functionReturn() {
    accept(RETURN);
    expression();
}

void Parser::ifStatement() {
    simpleIfStatement();
    if (symbol == ELSE) {
        accept(ELSE);
        statement();
    }
}

void Parser::simpleIfStatement() {
    accept(IF);
    accept(PARENOPEN);
    expression();
    accept(PARENCLOSE);
    accept(THEN);
    statement();
}

void Parser::whileStatement() {
    accept(WHILE);
    expression();
    accept(DO);
    statement();
}

void Parser::expression() {
    simpleExpression();
    if (has(relOp, symbol)) {
        nextSymbol();
        simpleExpression();
    }
}

void Parser::simpleExpression() {
    if (has(sign, symbol)) {
        nextSymbol();
    }
    term();
    while (has(addOp, symbol)) {
        nextSymbol();
        term();
    }
}

void Parser::term() {
    factor();
    while (has(multOp, symbol)) {
        nextSymbol();
        factor();
    }
}

void Parser::factor() {
    switch (symbol) {
        case IDENTIFIER:
            var();
            break;
        case FRACTCONST:
        case INTCONST:
        case CHARCONST:
            nextSymbol();
            break;
        case PARENOPEN:
            accept(PARENOPEN);
            expression();
            accept(PARENCLOSE);
            break;
        case NOTSIGN:
            accept(NOTSIGN);
            factor();
            break;
    }
}

void Parser::var() {
    accept(IDENTIFIER);
}