#include <iostream>
#include "Reader.h"
#include "Scanner.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "Interpreter.h"

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
    {VOIDTYPE,    "VOIDTYPE"},
    {STRINGTYPE,  "STRINGTYPE"},
    {INTEGERTYPE, "INTEGERTYPE"},
    {FRACTIONTYPE,"FRACTIONTYPE"},
    {BOOLEANTYPE, "BOOLEANTYPE,"},

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

void printErrorLine(const Scanner &scanner);
void printExceptionInfo(const Scanner &scanner, const Parser &parser);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Nie podano nazwy pliku wejsciowego" << std::endl;
        return 0;
    }
    Reader reader(argv[1]);
    Scanner scanner(&reader);
    Parser parser(scanner);
    SemanticAnalyzer semAnalyzer;

    ProgramNode *tree = nullptr;
    try {
        tree = parser.parse();
        std::cout << "*** No lexical errors ***\n" << std::endl;
        std::cout << "*** No syntax errors ***\n" << std::endl;
    } catch (ScannerException e) {
        std::cout << e.what();
        printErrorLine(scanner);
        return 0;
    } catch (ParseException e) {
        std::cout << e.what();
        printExceptionInfo(scanner, parser);

        if (scanner.getCurrentSymbol() == IDENTIFIER) {
            std::cout << scanner.getLastString() << std::endl;
        }
        return 0;
    }

    if (tree) {
        try {
            semAnalyzer.visit(tree);
            std::cout << "*** No semantic errors ***\n" << std::endl;
        } catch (ParseException e) {
            std::cout << e.what() << std::endl;
            return 0;
        }
    }
    std::cout << "***********************" << std::endl;
    std::cout << "Interpreting...\n" << std::endl;
    Interpreter interpreter(semAnalyzer.getPrototypes(), tree);
    try {
        interpreter.interpret();
    } catch (std::runtime_error e) {
        std::cout<< e.what() << std::endl;
    }

    // cleaning
    {
        delete tree;
    }
    return 0;
}

void printErrorLine(const Scanner &scanner) {
    std::cout << " in line: " << scanner.getLine() << std::endl;
}

void printExceptionInfo(const Scanner &scanner, const Parser &parser) {
    std::cout << ": " << mappings[parser.getCurrSymbol()];
    printErrorLine(scanner);

    if (scanner.getCurrentSymbol() == IDENTIFIER) {
        std::cout << scanner.getLastString() << std::endl;
    }
}