#include "error.h"

#include <utility>

error::error(std::string message_) {
    message = std::move(message_);
}

std::string error::getMessage() const {
    return message;
}

fraction_error::fraction_error(std::string message) : error(message) {
}

matrix_error::matrix_error(std::string message) : error(message) {
}

resistive_network_error::resistive_network_error(std::string message) : error(message) {
}
