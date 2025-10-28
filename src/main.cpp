#include <iostream>
#include "../include/mqtt.h"
#include "../include/com_ports.h" // Add this line

#ifdef ENABLE_WINDOWS_WINRT
#include "../include/ble.h"
#endif

int run_calc(int agc, char **argv);

int main(int argc, char *argv[])
{
	run_calc(argc, argv);
	run_mqtt();

#ifdef _WIN32
	com_ports::run_com_ports(); // Add this line
#endif

#ifdef ENABLE_WINDOWS_WINRT
	run_ble_scan();
#endif

	return 0;
}