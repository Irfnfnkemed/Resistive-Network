#ifndef RESISTIVE_NETWORK_FRACTION_H
#define RESISTIVE_NETWORK_FRACTION_H

#include <iostream>

class fraction {
private:
    long long numerator, denominator;
    bool sign;

    long long gcd(long long lhs, long long rhs);

    void reduction();

public:
    fraction();

    fraction(long long num);

    fraction(long long numerator_, long long denominator_);

    fraction(long long numerator_, long long denominator_, bool sign_);

    friend std::istream &operator>>(std::istream &in, fraction &obj);

    friend std::ostream &operator<<(std::ostream &out, const fraction &obj);

    friend fraction operator+(const fraction &lhs, const fraction &rhs);

    friend fraction operator-(const fraction &lhs, const fraction &rhs);

    friend fraction operator*(const fraction &lhs, const fraction &rhs);

    friend fraction operator/(const fraction &lhs, const fraction &rhs);

    fraction &operator=(const fraction &obj);

    bool operator==(const fraction &obj) const;

};

#endif //RESISTIVE_NETWORK_FRACTION_H
