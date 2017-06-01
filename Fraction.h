//
// Class representing a mixed number (fraction)
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_FRACTION_H
#define FRACTUS_FRACTION_H

#include <iostream>

class Fraction {
public:
    int whole;
    int numerator;
    int denominator;

    Fraction();
    ~Fraction();

    friend std::ostream& operator<<(std::ostream& os, const Fraction& obj);
};


#endif //FRACTUS_FRACTION_H
