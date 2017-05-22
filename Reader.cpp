//
// Reader source file
// Wiktor Franus, WUT 2017
//

#include <iostream>
#include "Reader.h"
Reader::Reader(const std::string &fileName) {
    fileStream.open(fileName, std::fstream::in);
    if (!fileStream.is_open()) {
        std::cout << "Nie mozna otworzyc pliku o nazwie " << fileName << std::endl;
    }
    streamIterator = new std::istreambuf_iterator<char>(fileStream);
}

Reader::~Reader() {
    delete streamIterator;
    fileStream.close();
}

char Reader::nextChar() {
    char c = **streamIterator;
    ++*streamIterator;
    if (c == '\n') {
        ++line;
    }
    return c;
}