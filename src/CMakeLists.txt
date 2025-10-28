# Calculator library
add_library(calculator calculator.cpp)
target_include_directories(calculator PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Run_calc library
add_library(run_calc run_calc.cpp)
target_link_libraries(run_calc PUBLIC calculator)

# MQTT handler
add_library(my_mqtt mqtt.cpp)
target_include_directories(my_mqtt PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(my_mqtt
    PUBLIC
        fmt::fmt
        PahoMqttCpp::paho-mqttpp3
)

# COM Ports lister (Windows only)
if(WIN32)
    add_library(my_com_ports com_ports.cpp)
    target_include_directories(my_com_ports PUBLIC ${CMAKE_SOURCE_DIR}/include)
endif()

# BLE scanner using WinRT (Windows only)
if(ENABLE_WINDOWS_WINRT)
    message(STATUS "ENABLE_WINDOWS_WINRT preprocessor macro defined")
    add_library(my_ble ble.cpp)
    target_include_directories(my_ble PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(my_ble
        PUBLIC
            fmt::fmt
            Microsoft::CppWinRT
            windowsapp
    )
endif()