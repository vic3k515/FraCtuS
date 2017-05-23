//
// Parser for FraCtuS language
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_PARSER_H
#define FRACTUS_PARSER_H

#include <set>

#include "Scanner.h"

class ParseException : std::exception {
public:
    ParseException(std::string msg) : exception(), err(msg) {};

    const char* what() const throw() {
      return err.c_str();
    }

  private:
    std::string err;
};


class Parser {
public:
    using SymSet = std::set<Token>;

    Parser(Scanner &scanner);
    void parse();
    Token getCurrSymbol() {
        return symbol;
    }

private:
    void accept(const Token& tkn);
    void accept(const SymSet& sset);
    bool has(const SymSet &sset, const Token &tkn);
    void nextSymbol();
    void block();
    void variablePart();
    void variableDeclaration();
    void varType();
    void type();
    void functionPart();
    void functionDeclaration();
    void compositeStatement();
    void statement();
    void assignment();
    void functionCall();
    void functionReturn();
    void ifStatement();
    void simpleIfStatement();
    void whileStatement();
    void expression();
    void simpleExpression();
    void term();
    void factor();
    void var();

    SymSet  relOp,
            addOp,
            multOp,
            sign,
            varTypes,
            retTypes;

    Scanner &scanner;
    Token symbol;
};

#endif //FRACTUS_PARSER_H
