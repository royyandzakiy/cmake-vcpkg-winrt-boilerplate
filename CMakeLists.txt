cmake_minimum_required(VERSION 3.20)

project(cmake_project_template LANGUAGES CXX)

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

# Windows-specific dependencies
if(WIN32)
    find_package(cppwinrt CONFIG REQUIRED)
endif()

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)

# Final executable
add_executable(cmake_project_template src/main.cpp)
target_link_libraries(cmake_project_template PRIVATE run_calc my_mqtt)

# Conditionally add BLE library on Windows
if(WIN32)
    target_link_libraries(cmake_project_template PRIVATE my_ble)
endif()