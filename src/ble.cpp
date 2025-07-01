#ifdef ENABLE_WINDOWS_WINRT

#include "../include/ble.h"

void run_ble_scan()
{
	try
	{
		winrt::init_apartment();

		std::cout << "Starting BLE scan (will run for 3 seconds)..." << std::endl;

		BluetoothLEAdvertisementWatcher watcher;
		std::promise<void> scanCompletedPromise;
		auto scanCompletedFuture = scanCompletedPromise.get_future();

		watcher.ScanningMode(BluetoothLEScanningMode::Active);

		watcher.Received([&](const auto &sender, const BluetoothLEAdvertisementReceivedEventArgs &args)
						 {
            std::cout << "\nBLE Device Found:" << std::endl;
            std::cout << "  Address: " << std::hex << args.BluetoothAddress() << std::dec << std::endl;
            std::cout << "  RSSI: " << args.RawSignalStrengthInDBm() << " dBm" << std::endl;

            auto advertisement = args.Advertisement();
            if (!advertisement.LocalName().empty())
            {
                std::cout << "  Name: " << winrt::to_string(advertisement.LocalName()) << std::endl;
            }
            else
            {
                auto getDeviceName = [args]() -> winrt::fire_and_forget {
                    try
                    {
                        auto device = co_await BluetoothLEDevice::FromBluetoothAddressAsync(args.BluetoothAddress());
                        if (device != nullptr && !device.Name().empty())
                        {
                            std::cout << "  Name: " << winrt::to_string(device.Name()) << std::endl;
                        }
                    }
                    catch (...) {} // Ignore errors in async name fetching
                };
                getDeviceName();
            } });

		watcher.Stopped([&](const auto &sender, const BluetoothLEAdvertisementWatcherStoppedEventArgs &args)
						{
            std::cout << "BLE scan stopped. Reason: " << static_cast<int>(args.Error()) << std::endl;
            scanCompletedPromise.set_value(); });

		watcher.Start();
		std::cout << "BLE scan started..." << std::endl;

		// Set up a timer to stop after 3 seconds
		std::thread([&watcher, &scanCompletedPromise]()
					{
            std::this_thread::sleep_for(std::chrono::seconds(3));
            if (watcher.Status() == BluetoothLEAdvertisementWatcherStatus::Started) {
                watcher.Stop();
            } })
			.detach();

		scanCompletedFuture.wait();
		std::cout << "BLE scan completed after 3 seconds." << std::endl;
	}
	catch (const winrt::hresult_error &ex)
	{
		std::cerr << "BLE scan error: " << winrt::to_string(ex.message()) << std::endl;
	}
	catch (const std::exception &ex)
	{
		std::cerr << "Error: " << ex.what() << std::endl;
	}
}

#endif // ENABLE_WINDOWS_WINRT