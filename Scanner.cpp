//
// Scanner source file
// Wiktor Franus, WUT 2017
//

#include <climits>
#include "Scanner.h"

Scanner::Scanner(Reader *source) : isCharPreloaded(false), errorOccurred(false) {
    this->source = source;
}

Token Scanner::nextSymbol() {
    do {
        if (isCharPreloaded) {
            isCharPreloaded = false;
        } else {
            c = source->nextChar();
        }
        if (c == '\0' || c == EOF) {
            return END_OF_FILE;
        }
        // skip comment in current line
        if (c == '#') {
            do c = source->nextChar();
            while (c != '\n');
        }
    } while (isspace(c) || c == '#');

    // keyword or identifier
    if (isalpha(c)) {
        lastString.clear();
        do {
            lastString.push_back(c);
            c = source->nextChar();
        } while (isalnum(c));
        isCharPreloaded = true;

        auto kwtoken = KeyWords.find(lastString);
        if (kwtoken != KeyWords.end()) {
            return (*kwtoken).second;
        } else {
            return IDENTIFIER;
        }
    }

    // integer or fraction
    if (c == '-' || isdigit(c)) {
        isCharPreloaded = true;
        if (c == '-') {
            c = source->nextChar();
        }
        if (isdigit(c)) {
            readInt(lastNumber);
            // int in lastNumber can be INTCONST
            // or a whole part of a mixed number,
            // or a numerator of a simple fraction
            // c is already loaded in readInt
            if (c == '.') {
                c = source->nextChar();
                if (isdigit(c)) {
                    Fraction f;
                    f.whole = lastNumber;
                    readInt(f.numerator);
                    if (c == '_') {
                        c = source->nextChar();
                        readInt(f.denominator);
                        lastFraction = f;
                        if (f.denominator == 0) {
                            ScanError("Mianownik musi być różny od 0!");
                        }
                        return FRACTCONST;
                    } else {
                        ScanError("Bledna stala ulamkowa: brak symbolu '_'");
                        return INTCONST;
                    }
                } else {
                    ScanError("Bledna stala ulamkowa: brak liczby po znaku '.'");
                    return INTCONST;
                }
            } else if (c == '_'){
                Fraction f;
                f.whole = 0;
                f.numerator = lastNumber;
                c = source->nextChar();
                readInt(f.denominator);
                lastFraction = f;
                return FRACTCONST;
            } else {
                return INTCONST;
            }
        } else {
            isCharPreloaded = true;
            return MINUS;
        }
    }
    if (c == '"') {
        lastString.clear();
        c = source->nextChar();
        if (c == '"') {
            c = source->nextChar();
            if (c != '"') ScanError("Bledna stala znakowa: za dużo znaków '\"'");
            return CHARCONST;
        }

        do {
            lastString.push_back(c);
            c = source->nextChar();
        } while (c != '"');

        c = source->nextChar();
        if (c == '"') {
            ScanError("Bledna stala znakowa: za dużo znaków '\"'");
            c = source->nextChar();
        }
        isCharPreloaded = true;
        return CHARCONST;
    }
    if (c == ';') {
        return SEMICOLON;
    }
    if (c == ':') {
        return COLON;
    }
    if (c == ',')
        return COMMA;
    if (c == '.')
        return PERIOD;
    if (c == '{') {
        return BRACEOPEN;
    }
    if (c == '}') {
        return BRACECLOSE;
    }
    if (c == '(') {
        return PARENOPEN;
    }
    if (c == ')') {
        return PARENCLOSE;
    }
    if (c == '!') {
        c = source->nextChar();
        if (c == '=') {
            c = source->nextChar();
            if (c == '=') {
                return NEQOP;
            }
        } else {
            isCharPreloaded = true;
            return NOTSIGN;
        }
    }
    if (c == '+') {
        return PLUS;
    }
    if (c == '-') {
        return MINUS;
    }
    if (c == '*') {
        return MULTSIGN;
    }
    if (c == '/') {
        return DIVSIGN;
    }
    if (c == '=') {
        c = source->nextChar();
        if (c == '=') {
            return EQOP;
        }
        isCharPreloaded = true;
        return EQALSIGN;
    }
    if (c == '<') {
        c = source->nextChar();
        if (c == '=') {
            return LEOP;
        }
        isCharPreloaded = true;
        return LTOP;
    }
    if (c == '>') {
        c = source->nextChar();
        if (c == '=') {
            return GEOP;
        }
        isCharPreloaded = true;
        return GTOP;
    }
    return OTHERS;
}

bool Scanner::readInt(int &num) {
    bool big = false;
    unsigned long long ul = 0;
    do {
        ul = ul * 10 + (c - '0');
        big = big || ul > INT_MAX;
        c = source->nextChar();
    } while (isdigit(c));

    num = (int) ul;
    if (big) {
        ScanError("Przekroczony zakres stalej calkowitej");
        return false;
    }
    return true;
}