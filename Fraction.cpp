//
// Fraction source file
// Wiktor Franus, WUT 2017
//

#include "Fraction.h"

Fraction::Fraction() {}

Fraction::~Fraction() {}

std::ostream& operator<<(std::ostream& os, const Fraction& obj) {
    os << obj.whole << "." << obj.numerator << "_" << obj.denominator;
    return os;
}