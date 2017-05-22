//
// Class for reading chars from input file
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_READER_H
#define FRACTUS_READER_H

#include <fstream>

class Reader {
public:
    Reader(const std::string &fileName);
    ~Reader();

    char nextChar();

    int getLine() {
        return line;
    }
private:
    std::fstream fileStream;
    std::istreambuf_iterator<char> *streamIterator;
    int line = 1;
};


#endif //FRACTUS_READER_H
