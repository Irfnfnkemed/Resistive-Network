#ifndef RESISTIVE_NETWORK_MATRIX_H
#define RESISTIVE_NETWORK_MATRIX_H

#include "fraction.h"

class matrix {
private:
    int m, n;
    fraction **data;

    void free_space();

public:
    matrix();

    matrix(int m_, int n_);

    matrix(fraction num);

    matrix(const matrix &obj);

    matrix(matrix &&obj) noexcept;

    ~matrix();

    matrix &operator=(const matrix &obj);

    matrix &operator=(matrix &&obj) noexcept;

    friend matrix operator*(const matrix &lhs, const matrix &rhs);

    fraction &operator()(int i, int j);

    friend std::istream &operator>>(std::istream &in, matrix &obj);

    friend std::ostream &operator<<(std::ostream &out, const matrix &obj);

    matrix transposition();

    fraction determination();

    matrix cofactor(int row, int col);

    void clear_data();

    void clear_all();
};

#endif //RESISTIVE_NETWORK_MATRIX_H
