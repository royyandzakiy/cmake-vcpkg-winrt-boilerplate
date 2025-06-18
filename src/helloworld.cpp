#include "../include/helloworld.h"

void to_print() {
    winrt::hstring winrt_string = L"Hello from WinRT!";
    fmt::print("WinRT string: {}\n", winrt::to_string(winrt_string));
}

namespace mqtt {
    const std::string message::EMPTY_STR;
}

void mqtt_connect() {
    const std::string SERVER_ADDRESS = "test.mosquitto.org";
    const std::string CLIENT_ID = "hello_world_mqtt_tester";

    mqtt::async_client mqtt_client(SERVER_ADDRESS, CLIENT_ID);
    fmt::print("MQTT client instance created (intentionally not connected): Address={}, ClientID={}\n",
               SERVER_ADDRESS, CLIENT_ID);

    mqtt::message_ptr msg = mqtt::make_message("topic/test", "Hello MQTT linker!");
    fmt::print("MQTT message created (intentionally not sent): Topic={}, Payload={}\n",
               msg->get_topic(), msg->get_payload_str());
}