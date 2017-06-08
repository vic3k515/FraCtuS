//
// Fraction source file
// Wiktor Franus, WUT 2017
//

#include "Fraction.h"

Fraction::Fraction()
: whole(0)
, numerator(0)
, denominator(1)
{}

Fraction::~Fraction() {}

std::ostream& operator<<(std::ostream& os, const Fraction& obj) {
    if (obj.whole) {
        os << obj.whole << ".";
    }
    os << obj.numerator << "_" << obj.denominator;
    return os;
}

std::istream& operator>>(std::istream& in, Fraction& obj) {
    std::string s;
    size_t periodPos, sepPos;
    in >> s;
    if ((periodPos = s.find('.')) != std::string::npos) {
        obj.whole = stoi(s.substr(0, periodPos));
    }
    if ((sepPos = s.find('_')) != std::string::npos) {
        if (periodPos == std::string::npos) {
            obj.numerator = stoi(s.substr(0, sepPos));
        } else {
            obj.numerator = stoi(s.substr(periodPos + 1, sepPos - periodPos - 1));
        }
        obj.denominator = stoi(s.substr(sepPos + 1, s.length() - sepPos - 1));
    }
    return in;
}

bool operator==(const Fraction &left, const Fraction &right) {
    Fraction l = left;
    Fraction r = right;

    convertToCommonDenominator(l,r);
    if (l.whole) l.numerator = l.whole * l.numerator;
    if (r.whole) r.numerator = r.whole * r.numerator;
    return l.numerator == r.numerator;
}

bool operator!=(const Fraction &left, const Fraction &right) {
    return !(left == right);
}

bool operator<(const Fraction &left, const Fraction &right) {
    Fraction l = left;
    Fraction r = right;

    convertToCommonDenominator(l,r);
    if (l.whole) l.numerator = l.whole * l.numerator;
    if (r.whole) r.numerator = r.whole * r.numerator;
    return l.numerator < r.numerator;
}

bool operator>(const Fraction &left, const Fraction &right) {
    Fraction l = left;
    Fraction r = right;

    convertToCommonDenominator(l,r);
    if (l.whole) l.numerator = l.whole * l.numerator;
    if (r.whole) r.numerator = r.whole * r.numerator;
    return l.numerator > r.numerator;
}

bool operator<=(const Fraction &left, const Fraction &right) {
    return !(left > right);
}

bool operator>=(const Fraction &left, const Fraction &right) {
    return !(right < left);
}

Fraction operator+(const Fraction &left, const Fraction &right) {
    Fraction l = left;
    Fraction r = right;
    Fraction ret;

    convertToCommonDenominator(l,r);
    ret.whole = l.whole + r.whole;
    ret.numerator = l.numerator + r.numerator;
    ret.denominator = l.denominator;
    return ret;
}

Fraction operator-(const Fraction &left, const Fraction &right) {
    Fraction l = left;
    Fraction r = right;
    Fraction ret;

    convertToCommonDenominator(l,r);
    ret.whole = l.whole - r.whole;
    ret.numerator = l.numerator - r.numerator;
    ret.denominator = l.denominator;
    return ret;
}

Fraction operator*(const Fraction &left, const Fraction &right) {
    Fraction ret;
    ret.whole = 0;
    ret.numerator = left.whole ? (left.whole * left.denominator) + left.numerator : left.numerator;
    ret.numerator *= right.whole ? (right.whole * right.denominator) + right.numerator : right.numerator;
    ret.denominator = left.denominator * right.denominator;
    return ret;
}

Fraction operator/(const Fraction &left, const Fraction &right) {
    Fraction r = right;
    Fraction ret;
    ret.whole = 0;
    ret.numerator = left.whole ? (left.whole * left.denominator) + left.numerator : left.numerator;
    r.numerator = right.denominator;
    r.denominator = right.whole ? (right.whole * right.denominator) + right.numerator : right.numerator;
    ret.numerator *= r.numerator;
    ret.denominator = left.denominator * r.denominator;
    return ret;
}

void convertToCommonDenominator(Fraction &l, Fraction &r) {
    if (l.denominator != r.denominator) {
        int mcm = mostCommonMultiple(l.denominator, r.denominator);
        l.numerator = (l.numerator * mcm) / l.denominator;
        r.numerator = (r.numerator * mcm) / r.denominator;
        l.denominator = r.denominator = mcm;
    }
}

int mostCommonMultiple(int a, int b) {
    int m = a*b;

    do
    {
        if(a>b) a=a-b;
        else b=b-a;
    }
    while(a!=b);

    return m/a;
}
