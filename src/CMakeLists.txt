# src/CMakeLists.txt

add_executable(HelloWorld 
    helloworld.cpp
    main.cpp
)

target_link_libraries(HelloWorld
    PRIVATE
        windowsapp # REQUIRED!
        fmt::fmt
        PahoMqttCpp::paho-mqttpp3
        Microsoft::CppWinRT
)