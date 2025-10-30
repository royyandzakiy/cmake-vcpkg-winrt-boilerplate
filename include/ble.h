#pragma once

#ifdef ENABLE_WINDOWS_WINRT

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Devices.Bluetooth.h>
#include <winrt/Windows.Devices.Bluetooth.Advertisement.h>
#include <chrono>
#include <future>

using namespace winrt;
using namespace Windows::Devices::Bluetooth;
using namespace Windows::Devices::Bluetooth::Advertisement;
using namespace Windows::Foundation;

void run_ble_scan();

#endif // ENABLE_WINDOWS_WINRT