#include <iostream>
#include "../include/mqtt.h"

#ifdef _WIN32
#include "../include/ble.h"
#endif

int run_calc(int agc, char **argv);

int main()
{
	run_calc(0, nullptr);
	run_mqtt();

#ifdef _WIN32
	run_ble_scan();
#endif

	return 0;
}