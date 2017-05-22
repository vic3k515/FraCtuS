#include <iostream>
#include "Reader.h"
#include "Scanner.h"

std::map<Token, std::string> mappings = {
    {PROGRAM,     "PROGRAM"},
    {VAR,         "VAR"},
    {BEGIN,       "BEGIN"},
    {END,         "END"},
    {RETURN,      "RETURN"},
    {IF,          "IF"},
    {THEN,        "THEN"},
    {ELSE,        "ELSE"},
    {DO,          "DO"},
    {WHILE,       "WHILE"},

    {IDENTIFIER,  "IDENTIFIER"},
    {INTCONST,    "INTCONST"},
    {FRACTCONST,  "FRACTCONST"},
    {CHARCONST,   "CHARCONST"},
    {SEMICOLON,   "SEMICOLON"},
    {COLON,       "COLON"},
    {COMMA,       "COMMA"},
    {PERIOD,      "PERIOD"},
    {BRACEOPEN,   "BRACEOPEN"},
    {BRACECLOSE,  "BRACECLOSE"},
    {PARENOPEN,   "PARENOPEN"},
    {PARENCLOSE,  "PARENCLOSE"},
    {EQALSIGN,    "EQALSIGN"},
    {NOTSIGN,     "NOTSIGN"},
    {PLUS,        "PLUS"},
    {MINUS,       "MINUS"},
    {MULTSIGN,    "MULTSIGN"},
    {DIVSIGN,     "DIVSIGN"},
    {EQOP,        "EQOP"},
    {NEQOP,       "NEQOP"},
    {LTOP,        "LTOP"},
    {GTOP,        "GTOP"},
    {LEOP,        "LEOP"},
    {GEOP,        "GEOP"},
    {OROP,        "OROP"},
    {ANDOP,       "ANDOP"},
    {END_OF_FILE, "END_OF_FILE"},
    {OTHERS,      "OTHERS"}
};

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Nie podano nazwy pliku wejsciowego" << std::endl;
        return 0;
    }
    Reader *reader = new Reader(argv[1]);
    Scanner *scanner = new Scanner(reader);

    Token t;
    while (!(scanner->errorOccured()) && ((t = scanner->nextSymbol()) != END_OF_FILE)) {
        std::cout << mappings[t];
        if (t == FRACTCONST) {
            std::cout << ": " << scanner->getLastFraction();
        }
        if (t == IDENTIFIER || t == CHARCONST) {
            std::cout << ": " << scanner->getLastString();
        }
        if (t == INTCONST) {
            std::cout << ": " << scanner->getLastNumber();
        }
        std::cout << std::endl;
    }

    delete reader;
    delete scanner;
    return 0;
}