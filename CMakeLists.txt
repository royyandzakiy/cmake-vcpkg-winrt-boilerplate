cmake_minimum_required(VERSION 3.10)

project(HelloWorld)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt CONFIG REQUIRED)
find_package(PahoMqttCpp CONFIG REQUIRED)
find_package(cppwinrt CONFIG REQUIRED)

add_subdirectory(src)
