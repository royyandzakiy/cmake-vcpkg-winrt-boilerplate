cmake_minimum_required(VERSION 3.20)

project(vcpkg_cmakepresets LANGUAGES CXX)

# Use VCPKG toolchain (set externally or via presets)
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
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
find_package(cppwinrt CONFIG REQUIRED)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(docs)

# Final executable
add_executable(vcpkg_proj src/main.cpp)
target_link_libraries(vcpkg_proj PRIVATE run_calc my_mqtt my_ble)
