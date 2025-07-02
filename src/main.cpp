#include <iostream>
#include "../include/mqtt.h"

#ifdef ENABLE_WINDOWS_WINRT
#include "../include/ble.h"
#endif
int run_calc(int agc, char **argv);

int main(int argc, char *argv[])
{
	run_calc(argc, argv);
	run_mqtt();

#ifdef ENABLE_WINDOWS_WINRT
	run_ble_scan();
#endif

	return 0;
}
