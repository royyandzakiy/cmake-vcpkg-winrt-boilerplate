#include "com_ports.h"

#ifdef _WIN32
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <iostream>
#include <iomanip>
#include <format>
#include <cfgmgr32.h>

#pragma comment(lib, "setupapi.lib")

namespace com_ports
{

	std::vector<ComPortInfo> list_com_ports()
	{
		std::vector<ComPortInfo> ports;

		HDEVINFO hDevInfo = SetupDiGetClassDevs(
			&GUID_DEVCLASS_PORTS,
			nullptr,
			nullptr,
			DIGCF_PRESENT);

		if (hDevInfo == INVALID_HANDLE_VALUE)
		{
			return ports;
		}

		SP_DEVINFO_DATA devInfoData;
		devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

		for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++)
		{
			ComPortInfo port_info;
			port_info.is_available = true;

			// Get device description (friendly name)
			CHAR deviceDesc[256];
			if (SetupDiGetDeviceRegistryPropertyA(
					hDevInfo,
					&devInfoData,
					SPDRP_FRIENDLYNAME,
					nullptr,
					(PBYTE)deviceDesc,
					sizeof(deviceDesc),
					nullptr))
			{

				port_info.description = deviceDesc;

				// Extract COM port number from description
				std::string desc_str = deviceDesc;
				size_t com_pos = desc_str.find("(COM");
				if (com_pos != std::string::npos)
				{
					size_t end_pos = desc_str.find(")", com_pos);
					if (end_pos != std::string::npos)
					{
						port_info.name = desc_str.substr(com_pos + 1, end_pos - com_pos - 1);
					}
				}
			}

			// If we couldn't extract COM port from friendly name, try device instance ID
			if (port_info.name.empty())
			{
				CHAR instanceId[256];
				if (SetupDiGetDeviceInstanceIdA(
						hDevInfo,
						&devInfoData,
						instanceId,
						sizeof(instanceId),
						nullptr))
				{

					std::string instance_str = instanceId;
					// Look for COM port in instance ID
					size_t com_pos = instance_str.find("COM");
					if (com_pos != std::string::npos)
					{
						port_info.name = "COM" + instance_str.substr(com_pos + 3);
						// Take only the COM port part
						size_t end_pos = port_info.name.find_first_of("&\\");
						if (end_pos != std::string::npos)
						{
							port_info.name = port_info.name.substr(0, end_pos);
						}
					}
				}
			}

			// Get device manufacturer
			CHAR manufacturer[256];
			if (SetupDiGetDeviceRegistryPropertyA(
					hDevInfo,
					&devInfoData,
					SPDRP_MFG,
					nullptr,
					(PBYTE)manufacturer,
					sizeof(manufacturer),
					nullptr))
			{

				if (!port_info.description.empty())
				{
					port_info.description += " - " + std::string(manufacturer);
				}
				else
				{
					port_info.description = manufacturer;
				}
			}

			// Only add if we found a valid COM port name
			if (!port_info.name.empty() && port_info.name.find("COM") != std::string::npos)
			{
				ports.push_back(port_info);
			}
		}

		SetupDiDestroyDeviceInfoList(hDevInfo);
		return ports;
	}

	// Alternative method using registry to enumerate COM ports
	std::vector<ComPortInfo> list_com_ports_registry()
	{
		std::vector<ComPortInfo> ports;

		HKEY hKey;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
						  "HARDWARE\\DEVICEMAP\\SERIALCOMM",
						  0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{

			DWORD index = 0;
			CHAR valueName[256];
			CHAR data[256];
			DWORD valueNameSize, dataSize, type;

			while (true)
			{
				valueNameSize = sizeof(valueName);
				dataSize = sizeof(data);

				LONG result = RegEnumValueA(hKey, index, valueName, &valueNameSize,
											nullptr, &type, (LPBYTE)data, &dataSize);

				if (result == ERROR_NO_MORE_ITEMS)
				{
					break;
				}

				if (result == ERROR_SUCCESS && type == REG_SZ)
				{
					ComPortInfo port_info;
					port_info.name = data;
					port_info.description = valueName;
					port_info.is_available = true;
					ports.push_back(port_info);
				}

				index++;
			}

			RegCloseKey(hKey);
		}

		return ports;
	}

	void print_com_ports()
	{
		std::cout << "=== Available COM Ports (PnP Enumeration) ===\n";

		auto ports = list_com_ports();

		if (ports.empty())
		{
			std::cout << "No COM ports found via PnP. Trying registry method...\n";
			ports = list_com_ports_registry();
		}

		if (ports.empty())
		{
			std::cout << "No COM ports found.\n";
			return;
		}

		std::cout << std::left << std::setw(10) << "Port"
				  << std::setw(50) << "Description"
				  << std::setw(12) << "Status" << "\n";
		std::cout << std::string(72, '-') << "\n";

		for (const auto &port : ports)
		{
			std::string status = port.is_available ? "Available" : "Unavailable";
			std::cout << std::setw(10) << port.name
					  << std::setw(50) << port.description
					  << std::setw(12) << status << "\n";
		}
	}

	int run_com_ports()
	{
		try
		{
			print_com_ports();
			return 0;
		}
		catch (const std::exception &e)
		{
			std::cerr << "Error listing COM ports: " << e.what() << "\n";
			return 1;
		}
	}

} // namespace com_ports

#endif