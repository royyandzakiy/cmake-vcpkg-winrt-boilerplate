#include "com_ports.h"

#ifdef _WIN32
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <format>

namespace com_ports {

std::vector<ComPortInfo> list_com_ports() {
    std::vector<ComPortInfo> ports;
    
    // Try common COM ports
    for (int i = 1; i <= 256; i++) {
        std::string port_name = std::format("COM{}", i);
        ComPortInfo port_info;
        port_info.name = port_name;
        port_info.is_available = false;
        
        HANDLE hCom = CreateFileA(
            port_name.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr
        );
        
        if (hCom != INVALID_HANDLE_VALUE) {
            port_info.is_available = true;
            
            // Try to get more info about the port
            DCB dcb = {0};
            dcb.DCBlength = sizeof(dcb);
            if (GetCommState(hCom, &dcb)) {
                port_info.description = std::format("Baud: {}, Data: {}, Stop: {}, Parity: {}",
                    dcb.BaudRate, dcb.ByteSize, dcb.StopBits, dcb.Parity);
            } else {
                port_info.description = "Available serial port";
            }
            
            CloseHandle(hCom);
        } else {
            DWORD error = GetLastError();
            if (error == ERROR_ACCESS_DENIED) {
                port_info.description = "Port exists but access denied";
                port_info.is_available = false;
            } else {
                continue; // Port doesn't exist
            }
        }
        
        ports.push_back(port_info);
    }
    
    return ports;
}

void print_com_ports() {
    std::cout << "=== Available COM Ports ===\n";
    
    auto ports = list_com_ports();
    
    if (ports.empty()) {
        std::cout << "No COM ports found.\n";
        return;
    }
    
    std::cout << std::left << std::setw(10) << "Port" 
              << std::setw(40) << "Description" 
              << std::setw(12) << "Status" << "\n";
    std::cout << std::string(62, '-') << "\n";
    
    for (const auto& port : ports) {
        std::string status = port.is_available ? "Available" : "Unavailable";
        std::cout << std::setw(10) << port.name 
                  << std::setw(40) << port.description 
                  << std::setw(12) << status << "\n";
    }
}

int run_com_ports() {
    try {
        print_com_ports();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error listing COM ports: " << e.what() << "\n";
        return 1;
    }
}

} // namespace com_ports

#endif