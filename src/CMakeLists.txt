# Calculator library
add_library(calculator calculator.cpp)
target_include_directories(calculator PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Run_calc library
add_library(run_calc run_calc.cpp)
target_link_libraries(run_calc PUBLIC calculator)

# BLE scanner using WinRT
add_library(my_ble ble.cpp)
target_include_directories(my_ble PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(my_ble
    PUBLIC
        fmt::fmt
        Microsoft::CppWinRT
        windowsapp  # Required for WinRT linker symbols
)

# MQTT handler
add_library(my_mqtt mqtt.cpp)
target_include_directories(my_mqtt PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(my_mqtt
    PUBLIC
        fmt::fmt
        PahoMqttCpp::paho-mqttpp3
)
