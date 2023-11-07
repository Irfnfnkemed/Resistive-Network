#include "resistive_network.h"
#include "error.h"

resistive_network::resistive_network() {
}

resistive_network::~resistive_network() {
}

void resistive_network::init(int interface_size_, int connection_size_) {
    if (connection_size_ < interface_size_ - 1) {
        throw resistive_network_error("The resistive network should be connected.");
    }
    interface_size = interface_size_;
    connection_size = connection_size_;
    interface_current = matrix(1, interface_size);
    interface_voltage = matrix(1, interface_size);
    adjacency = matrix(connection_size, interface_size);
    conduction = matrix(connection_size, connection_size);
}

void resistive_network::set_resistance() {
    if (fix_resistance) {
        throw resistive_network_error("There is already a connection. Clear first.");
    }
    std::cout << "You should set " << connection_size << " resistances.\n";
    std::cout << "Please type in the following format: [interface1] [interface2] [resistance]" << std::endl;
    int from, to;
    fraction resistance;
    for (int i = 0; i < connection_size; ++i) {
        std::cin >> from >> to >> resistance;
        if (from == to) {
            throw resistive_network_error(
                    "It is not allowed to connect both ends of the wiring to the same interface.");
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
    if (det == 0) {
        clear_all();
        throw resistive_network_error("The resistive network should be connected.");
    }
    fix_resistance = true;
}

fraction resistive_network::get_equivalent_resistance(int interface_id1, int interface_id2) {
    if (!fix_resistance) {
        throw resistive_network_error("The interfaces are not connected yet.");
    }
    if (interface_id1 >= interface_size || interface_id1 < 0 ||
        interface_id2 >= interface_size || interface_id2 < 0) {
        throw resistive_network_error("Invalid interface id.");
    }
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
        throw resistive_network_error("The currents have been set already.");
    }
    input_current = true;
    interface_current(0, id) = current;
}

fraction resistive_network::get_current() {
    if (!input_voltage) {
        throw resistive_network_error("The voltages have been set already.");
    }
    interface_current = (laplace * interface_voltage.transposition()).transposition();
    fix_current = true;
}

void resistive_network::display_current() {
    if (!fix_current) {
        get_current();
    }
    std::cout << "display current (regard inflow as positive):" << std::endl;
    for (int i = 0; i < interface_size; ++i) {
        std::cout << "interface " << i << " : " << interface_current(0, i) << std::endl;
    }
}

fraction resistive_network::set_voltage(int id, fraction voltage) {
    if (input_current) {
        throw resistive_network_error("The currents have been set already.");
    }
    if (id >= interface_size || id < 0) {
        throw resistive_network_error("Invalid interface id.");
    }
    input_voltage = true;
    interface_voltage(0, id) = voltage;
}

fraction resistive_network::get_voltage(int id, fraction voltage) {
    if (!input_current) {
        throw resistive_network_error("The voltages have been set already.");
    }
    fraction sum = 0;
    for (int i = 0; i < interface_size; ++i) {
        sum = sum + interface_current(0, i);
    }
    if (!(sum == 0)) {
        throw resistive_network_error("Invalid currents setting.");
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
        set_voltage(0, 0);
    }
    std::cout << "display voltage:" << std::endl;
    for (int i = 0; i < interface_size; ++i) {
        std::cout << "interface " << i << " : " << interface_voltage(0, i) << std::endl;
    }
}

fraction resistive_network::display_power() {
    if (!input_current && !input_voltage) {
        throw resistive_network_error("The currents and voltages are not set yet.");
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
