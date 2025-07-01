#include <iostream>
#include "../include/mqtt.h"
#include "../include/ble.h"

using namespace winrt;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::Advertisement;

int run_calc(int agc, char **argv);

int main()
{
	init_apartment(); // Initialize WinRT

	run_calc(0, nullptr);
	run_ble_scan();
	run_mqtt();

	return 0;
}