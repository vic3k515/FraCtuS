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

    std::string getLastString() {
        return lastString;
    }

    int getLine() {
        return source->getLine();
    }

    int getLastNumber() {
        return lastNumber;
    }

    Fraction getLastFraction() {
        return lastFraction;
    }

    char getCurrentSymbol() {
        return c;
    }

    bool errorOccured() {
        return errorOccurred;
    }

    void ScanError(const char *mtxt="", const char *atxt="") {
        std::cout << "Lexical error in line: " + std::to_string(getLine()) + " " + mtxt << std::endl;
        errorOccurred = true;
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
