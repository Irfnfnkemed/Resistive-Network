#include "matrix.h"


matrix::matrix() {
    m = n = 0;
    data = nullptr;
}

matrix::matrix(int m_, int n_) {
    m = m_;
    n = n_;
    data = new fraction *[m];
    for (int i = 0; i < m; ++i) {
        data[i] = new fraction[n];
        for (int j = 0; j < n; ++j) {
            data[i][j] = 0;
        }
    }
}

matrix::matrix(fraction num) {
    m = n = 1;
    data = new fraction *[1];
    data[0] = new fraction[1];
    data[0][0] = num;
}

matrix::matrix(const matrix &obj) {
    m = obj.m;
    n = obj.n;
    data = new fraction *[m];
    for (int i = 0; i < m; ++i) {
        data[i] = new fraction[n];
        for (int j = 0; j < n; ++j) {
            data[i][j] = obj.data[i][j];
        }
    }
}

matrix::matrix(matrix &&obj) noexcept {
    m = obj.m;
    n = obj.n;
    data = obj.data;
    obj.data = nullptr;
}

matrix &matrix::operator=(const matrix &obj) {
    if (this == &obj) {
        return *this;
    }
    free_space();
    m = obj.m;
    n = obj.n;
    data = new fraction *[m];
    for (int i = 0; i < m; ++i) {
        data[i] = new fraction[n];
        for (int j = 0; j < n; ++j) {
            data[i][j] = obj.data[i][j];
        }
    }
    return *this;
}

matrix &matrix::operator=(matrix &&obj) noexcept {
    free_space();
    m = obj.m;
    n = obj.n;
    data = obj.data;
    obj.data = nullptr;
    return *this;
}

matrix::~matrix() {
    free_space();
}

void matrix::free_space() {
    if (data != nullptr) {
        for (int i = 0; i < m; ++i) {
            delete[]data[i];
        }
        delete[]data;
    }
}

matrix matrix::transposition() {
    matrix trans = matrix(n, m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            trans.data[i][j] = data[j][i];
        }
    }
    return trans;
}

matrix operator*(const matrix &lhs, const matrix &rhs) {
    if (lhs.n != rhs.m) {
        throw std::exception();
    }
    matrix product = matrix(lhs.m, rhs.n);
    for (int i = 0; i < product.m; ++i) {
        for (int j = 0; j < product.n; ++j) {
            product.data[i][j] = 0;
        }
    }
    for (int i = 0; i < lhs.m; ++i) {
        for (int j = 0; j < rhs.n; ++j) {
            for (int k = 0; k < lhs.n; ++k) {
                product.data[i][j] = product.data[i][j] + lhs.data[i][k] * rhs.data[k][j];
            }
        }
    }
    return product;
}

fraction &matrix::operator()(int i, int j) {
    return data[i][j];
}

std::istream &operator>>(std::istream &in, matrix &obj) {
    for (int i = 0; i < obj.m; ++i) {
        for (int j = 0; j < obj.n; ++j) {
            in >> obj.data[i][j];
        }
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const matrix &obj) {
    for (int i = 0; i < obj.m; ++i) {
        for (int j = 0; j < obj.n; ++j) {
            out << obj.data[i][j] << '\t';
        }
        out << '\n';
    }
    return out;
}

fraction matrix::determination() {
    int swap_num = 0;
    fraction det = 1;
    if (m != n) {
        throw std::exception();
    }
    fraction **backup = new fraction *[n];
    for (int i = 0; i < n; ++i) {
        backup[i] = new fraction[n];
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            backup[i][j] = data[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            for (int k = j + 1; k < n; ++k) {
                data[i][k] = data[i][k] - data[i][j] * data[j][k];
            }
            data[i][j] = 0;
        }
        if (data[i][i] == 0) {
            for (int t = i + 1; t <= n; ++t) {
                if (i == n - 1 || t == n) {
                    return 0;//行列式为0
                }
                if (!(data[t][i] == 0)) {
                    std::swap(data[i], data[t]);//将非零行交换
                    ++swap_num;
                    break;
                }
            }
        }
        for (int j = 0; j < n; ++j) {
            if (j == i) {
                continue;
            }
            data[i][j] = data[i][j] / data[i][i];
        }
        det = det * data[i][i];
        data[i][i] = 1;
    }
    if (swap_num % 2) {
        det = -1 * det;
    }
    for (int i = 0; i < n; ++i) {
        delete[]data[i];
    }
    delete[]data;
    data = backup;
    return det;
}

matrix matrix::cofactor(int row, int col) {
    if (m != n) {
        throw std::exception();
    }
    matrix tmp = matrix(n - 1, n - 1);
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            tmp.data[i][j] = data[i][j];
        }
    }
    for (int i = row + 1; i < n; ++i) {
        for (int j = 0; j < col; ++j) {
            tmp.data[i - 1][j] = data[i][j];
        }
    }
    for (int i = 0; i < row; ++i) {
        for (int j = col + 1; j < n; ++j) {
            tmp.data[i][j - 1] = data[i][j];
        }
    }
    for (int i = row + 1; i < n; ++i) {
        for (int j = col + 1; j < n; ++j) {
            tmp.data[i - 1][j - 1] = data[i][j];
        }
    }
    return tmp;
}

void matrix::clear_data() {
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            data[i][j] = 0;
        }
    }
}

void matrix::clear_all() {
    m = n = 0;
    free_space();
}
