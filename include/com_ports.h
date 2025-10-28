#pragma once

#ifdef _WIN32
#include <string>
#include <vector>

struct ComPortInfo {
    std::string name;
    std::string description;
    bool is_available;
};

namespace com_ports {
    std::vector<ComPortInfo> list_com_ports();
    void print_com_ports();
    int run_com_ports();
}

#endif