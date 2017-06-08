//
// Lexer for FraCtuS language
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_SCANNER_H
#define FRACTUS_SCANNER_H

#include <map>
#include <iostream>
#include "Reader.h"
#include "Fraction.h"

enum Token {
    PROGRAM,
    VAR,
    BEGIN,
    END,
    RETURN,
    IF,
    THEN,
    ELSE,
    DO,
    WHILE,
    VOIDTYPE,
    STRINGTYPE,
    INTEGERTYPE,
    FRACTIONTYPE,
    BOOLEANTYPE,

    IDENTIFIER,
    INTCONST,
    FRACTCONST,
    CHARCONST,
    SEMICOLON,
    COLON,
    COMMA,
    PERIOD,
    BRACEOPEN,
    BRACECLOSE,
    PARENOPEN,
    PARENCLOSE,
    EQALSIGN,
    NOTSIGN,
    PLUS,
    MINUS,
    MULTSIGN,
    DIVSIGN,
    EQOP,
    NEQOP,
    LTOP,
    GTOP,
    LEOP,
    GEOP,
    OROP,
    ANDOP,
    END_OF_FILE,
    OTHERS
};

class ScannerException : public std::runtime_error {
public:
    ScannerException(std::string msg) : runtime_error(msg) {};
};

class Scanner {
public:
    std::map<std::string, Token> KeyWords = {
            {"program", PROGRAM},
            {"var",     VAR},
            {"begin",   BEGIN},
            {"end",     END},
            {"return",  RETURN},
            {"if",      IF},
            {"then",    THEN},
            {"else",    ELSE},
            {"do",      DO},
            {"while",   WHILE},
            {"or",      OROP},
            {"and",     ANDOP},
            {"void",    VOIDTYPE},
            {"string",  STRINGTYPE},
            {"integer", INTEGERTYPE},
            {"fraction",FRACTIONTYPE},
            {"boolean", BOOLEANTYPE},
    };

    Scanner(Reader*);
    ~Scanner() {};

    Token nextSymbol();

    std::string getLastString() const {
        return lastString;
    }

    int getLine() const {
        return source->getLine();
    }

    int getLastNumber() const {
        return lastNumber;
    }

    Fraction getLastFraction() const {
        return lastFraction;
    }

    char getCurrentSymbol() const {
        return c;
    }

    bool errorOccured() {
        return errorOccurred;
    }

    void ScanError(const std::string &mtxt="", const std::string &atxt="") {
        errorOccurred = true;
        throw ScannerException("Lexical error : " + mtxt);
    }
private:
    bool readInt(int& num);

    Reader *source;
    char c;
    bool isCharPreloaded;
    bool errorOccurred;
    int lastNumber;
    Fraction lastFraction;
    std::string lastString;
};


#endif //FRACTUS_SCANNER_H
