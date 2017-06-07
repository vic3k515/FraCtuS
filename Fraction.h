//
// Class representing a mixed number (fraction)
// Wiktor Franus, WUT 2017
//

#ifndef FRACTUS_FRACTION_H
#define FRACTUS_FRACTION_H

#include <iostream>
#include <utility>

class Fraction {
public:
    int whole;
    int numerator;
    int denominator;

    Fraction();
    ~Fraction();

    friend std::ostream& operator<<(std::ostream& os, const Fraction& obj);
    friend std::istream& operator>>(std::istream& in, Fraction& obj);
};

bool operator==(const Fraction &left, const Fraction &right);
bool operator!=(const Fraction &left, const Fraction &right);
bool operator<(const Fraction &left, const Fraction &right);
bool operator>(const Fraction &left, const Fraction &right);
bool operator<=(const Fraction &left, const Fraction &right);
bool operator>=(const Fraction &left, const Fraction &right);
Fraction operator+(const Fraction &left, const Fraction &right);
Fraction operator-(const Fraction &left, const Fraction &right);
Fraction operator*(const Fraction &left, const Fraction &right);
Fraction operator/(const Fraction &left, const Fraction &right);

void convertToCommonDenominator(Fraction &l, Fraction &r);
int mostCommonMultiple(int a, int b);

#endif //FRACTUS_FRACTION_H
