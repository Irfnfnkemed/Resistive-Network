#ifndef RESISTIVE_NETWORK_ERROR_H
#define RESISTIVE_NETWORK_ERROR_H

#include <exception>
#include <string>

class error : public std::exception {
private:

    std::string message;

public:

    explicit error(std::string message);

    std::string getMessage() const;
};

class fraction_error : public error {
public:
    explicit fraction_error(std::string message);
};

class matrix_error : public error {
public:
    explicit matrix_error(std::string message);
};

class resistive_network_error : public error {
public:
    explicit resistive_network_error(std::string message);
};


#endif //RESISTIVE_NETWORK_ERROR_H
