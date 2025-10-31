# =============================================================================
# Project Configuration
# =============================================================================
cmake_minimum_required(VERSION 3.20)

project(cmake_project_template 
    LANGUAGES CXX
    VERSION 1.0.0
)

# =============================================================================
# Platform Configuration
# =============================================================================
if(WIN32)
    include(cmake/setup-windows.cmake)
endif()

# =============================================================================
# WinRT Configuration (OPTIONAL - Easy to remove completely)
# =============================================================================
option(ENABLE_WINDOWS_WINRT "Enable Windows WinRT support" OFF) # change this to use WinRT
message(STATUS "Windows WinRT support: ${ENABLE_WINDOWS_WINRT}")

if(ENABLE_WINDOWS_WINRT)
    find_package(cppwinrt CONFIG REQUIRED)
    add_compile_definitions(ENABLE_WINDOWS_WINRT=1)
    message(STATUS "Building with Windows WinRT BLE support")
endif()

# =============================================================================
# Toolchain Configuration
# =============================================================================
if(NOT CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

if(CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using VCPKG toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# =============================================================================
# Compiler Configuration
# =============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =============================================================================
# Dependencies
# =============================================================================
find_package(GTest CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)

# =============================================================================
# Project Structure
# =============================================================================
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)

# =============================================================================
# Main Executable
# =============================================================================
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Core dependencies (always included)
target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
        run_calc 
        my_mqtt
)

# Platform-specific dependencies
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_com_ports)
endif()

# OPTIONAL: WinRT dependencies (easy to remove)
if(ENABLE_WINDOWS_WINRT)
    target_link_libraries(${PROJECT_NAME} PRIVATE my_ble)
endif()