#ifndef RESISTIVE_NETWORK_RESISTIVE_NETWORK_H
#define RESISTIVE_NETWORK_RESISTIVE_NETWORK_H

#include "matrix.h"

class resistive_network {
private:
    int interface_size;//1-based
    int connection_size;//1-based
    matrix interface_current;
    matrix interface_voltage;
    matrix adjacency;
    matrix conduction;
    matrix laplace;
    fraction det;//laplace矩阵去除第i行、第i列后的矩阵行列式（与i无关）
    bool input_current = false, input_voltage = false;//表示将输入电流/电压
    bool fix_current = false, fix_voltage = false;//表示电流/电压已经通过计算确定，可以打印

public:
    resistive_network(int interface_size_, int connection_size_);

    ~resistive_network();

    void set_resistance();

    fraction get_equivalent_resistance(int interface_id1, int interface_id2);

    fraction set_current(int id, fraction current);

    fraction get_current();

    void display_current();

    fraction set_voltage(int id, fraction voltage);

    fraction get_voltage(int id, fraction voltage);

    void display_voltage();

    fraction display_power();

    void clear_input();

    void clear_all();
};


#endif //RESISTIVE_NETWORK_RESISTIVE_NETWORK_H
