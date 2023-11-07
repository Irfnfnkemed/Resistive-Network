#include "resistive_network.h"

resistive_network::resistive_network(int interface_size_, int connection_size_) {
    interface_size = interface_size_;
    connection_size = connection_size_;
    interface_current = matrix(1, interface_size);
    interface_voltage = matrix(1, interface_size);
    adjacency = matrix(connection_size, interface_size);
    conduction = matrix(connection_size, connection_size);
}

resistive_network::~resistive_network() = default;

void resistive_network::set_resistance() {
    int from, to;
    fraction resistance;
    for (int i = 0; i < connection_size; ++i) {
        std::cin >> from >> to >> resistance;
        if (from == to) {
            throw std::exception();
        }
        if (from > to) {
            std::swap(from, to);
        }
        conduction(i, i) = 1 / resistance;
        adjacency(i, from) = -1;
        adjacency(i, to) = 1;
    }
    laplace = adjacency.transposition() * conduction * adjacency;
    det = laplace.cofactor(connection_size - 1, connection_size - 1).determination();
}

fraction resistive_network::get_equivalent_resistance(int interface_id1, int interface_id2) {
    if (interface_id1 == interface_id2) {
        return 0;
    }
    if (interface_id1 > interface_id2) {
        std::swap(interface_id1, interface_id2);
    }

    return laplace.cofactor(interface_id2, interface_id2).cofactor(interface_id1, interface_id1).determination() / det;
}

fraction resistive_network::set_current(int id, fraction current) {
    if (input_voltage) {
        throw std::exception();
    }
    input_current = true;
    interface_current(0, id) = current;
}

fraction resistive_network::get_current() {
    if (!input_voltage) {
        throw std::exception();
    }
    interface_current = (laplace * interface_voltage.transposition()).transposition();
    fix_current = true;
}

void resistive_network::display_current() {
    if (!fix_current) {
        throw std::exception();
    }
    std::cout << "display current (regard inflow as positive):" << std::endl;
    for (int i = 0; i < interface_size; ++i) {
        std::cout << "interface " << i << " : " << interface_current(0, i) << std::endl;
    }
}

fraction resistive_network::set_voltage(int id, fraction voltage) {
    if (input_current) {
        throw std::exception();
    }
    input_voltage = true;
    interface_voltage(0, id) = voltage;
}

fraction resistive_network::get_voltage(int id, fraction voltage) {
    if (!input_current) {
        throw std::exception();
    }
    fraction sum = 0;
    for (int i = 0; i < interface_size; ++i) {
        sum = sum + interface_current(0, i);
    }
    if (!(sum == 0)) {
        throw std::exception();
    }
    matrix tmp = laplace.cofactor(interface_size - 1, interface_size - 1);
    for (int j = 0; j < interface_size - 1; ++j) {
        for (int i = 0; i < interface_size - 1; ++i) {
            tmp(i, j) = interface_current(0, i);
        }
        interface_voltage(0, j) = tmp.determination() / det;
        for (int i = 0; i < interface_size - 1; ++i) {
            tmp(i, j) = laplace(i, j);
        }
    }
    interface_voltage(0, interface_size - 1) = 0;
    fraction offset = voltage - interface_voltage(0, id);
    for (int i = 0; i < interface_size; ++i) {
        interface_voltage(0, i) = interface_voltage(0, i) + offset;
    }
    fix_voltage = true;
}

void resistive_network::display_voltage() {
    if (!fix_voltage) {
        throw std::exception();
    }
    std::cout << "display voltage:" << std::endl;
    for (int i = 0; i < interface_size; ++i) {
        std::cout << "interface " << i << " : " << interface_voltage(0, i) << std::endl;
    }
}

fraction resistive_network::display_power() {
    if (!input_current && !input_voltage) {
        throw std::exception();
    }
    if (input_current && !fix_voltage) {
        get_voltage(0, 0);
    } else if (input_voltage && !fix_current) {
        get_current();
    }
    matrix volatile_resistance = adjacency * interface_voltage.transposition();
    fraction power = 0;
    for (int i = 0; i < connection_size; ++i) {
        power = power + volatile_resistance(i, 0) * volatile_resistance(i, 0) * conduction(i, i);
    }
    return power;
}

void resistive_network::clear_input() {
    interface_voltage.clear_data();
    interface_current.clear_data();
    input_current = input_voltage = fix_current = fix_voltage = false;
}

void resistive_network::clear_all() {
    interface_size = connection_size = 0;
    interface_voltage.clear_all();
    interface_current.clear_all();
    adjacency.clear_all();
    conduction.clear_all();
    laplace.clear_all();
    det = 0;
    input_current = input_voltage = fix_current = fix_voltage = false;
}
