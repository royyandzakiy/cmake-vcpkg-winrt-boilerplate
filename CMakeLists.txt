cmake_minimum_required(VERSION 3.20)

project(cmake_project_template LANGUAGES CXX)

if(WIN32)
    include(cmake/setup-windows.cmake)
endif()

if(NOT DEFINED ENABLE_WINDOWS_WINRT)
    if(WIN32)
		set(ENABLE_WINDOWS_WINRT ON)
    else()
        set(ENABLE_WINDOWS_WINRT OFF)
    endif()
else()
    message(STATUS "ENABLE_WINDOWS_WINRT defined externally: ${ENABLE_WINDOWS_WINRT}")
endif()

message(STATUS "Final ENABLE_WINDOWS_WINRT value: ${ENABLE_WINDOWS_WINRT}")
message(STATUS "WIN32: ${WIN32}")

# Use VCPKG toolchain (set externally or via presets)
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
    message(STATUS "VCPKG_ROOT: $ENV{VCPKG_ROOT}")
    message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# C++ settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optional: Enable Clang-Tidy if defined
if(CLANGTIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
endif()

# Dependencies (resolved through vcpkg)
find_package(fmt CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)

if(ENABLE_WINDOWS_WINRT)
    find_package(cppwinrt CONFIG REQUIRED)
    message(STATUS "Building with BLE support")
    add_compile_definitions(ENABLE_WINDOWS_WINRT=1)
else()
    message(STATUS "Building without BLE support")
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)

# Final executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE run_calc my_mqtt)

# Conditionally add COM ports library on Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_com_ports)
endif()

# Conditionally add BLE library on Windows
if(ENABLE_WINDOWS_WINRT)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_ble)
endif()