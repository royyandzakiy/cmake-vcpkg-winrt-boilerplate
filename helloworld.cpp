#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <future>
#include <cstdlib> 
#include <iomanip>
#include <sstream>
#include <memory>
#include <regex>

// #undef max
#include <mqtt/async_client.h>
#include <fmt/core.h>

#include <windows.h>
#include <tlhelp32.h>

// ========================================================================================================
// config.h
// ========================================================================================================

#include <string>
#include <vector>
#include <chrono>

// ============== NEXUS CONFIG ==============

std::string SERVER_ADDRESS = "test.mosquitto.org:1883";
const std::string DEVICE_SN = "MK3:1073003434";

const std::string CLIENT_ID = "nexus_client_id_" + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()); // Unique ID
const std::string TOPIC_SUB_SENSORDATA = "nexus/sensordata/" + DEVICE_SN;
const std::string TOPIC_SUB_PROTOCOLDATA = "nexus/protocoldata/" + DEVICE_SN;
const std::string TOPIC_PUB_PROTOCOLDATA = "server/protocoldata/" + DEVICE_SN;
const int QOS = 0; // Quality of Service: 0 (at most once), 1 (at least once), 2 (exactly once)
const bool TO_RETAIN = false;

// ============== NEXUS COMMANDS ==============

const std::vector<uint8_t> start_streaming_cmd = {
            0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
};
const std::vector<uint8_t> stop_streaming_cmd = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const std::vector<uint8_t> get_device_info_cmd = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const std::vector<uint8_t> get_calibration_cmd = {
    0xFF, 0xFF, 0xFF, 0xFF, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


// ========================================================================================================
// MqttHandler.h
// ========================================================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <future>
#include <cstdlib> 
#include <mqtt/async_client.h>
#include <iomanip>
#include <sstream>
#include <memory>

/*
* examples:
* - mosquitto_sub -h test.mosquitto.org -p 1883 -t "pub/nexus"
* - mosquitto_pub -h localhost -p 1883 -t sub/nexus -m "testdsfsdf"
* - mosquitto_pub -h 192.168.1.5 -p 1883 -t sub/nexus -m "testdsfsdf"
*/

namespace mqtt {
    const std::string message::EMPTY_STR;
}

// ============== MQTT_HANDLER CLASS ==============

std::string hexToString(const void* data, size_t length);
class myaction_listener : public virtual mqtt::iaction_listener
{
    std::string name_;
    std::atomic<bool>& connected_flag;
    std::promise<void> prom_;
    std::atomic<bool> promise_set_{ false }; // New flag

public:
    myaction_listener(const std::string& name, std::atomic<bool>& flag)
        : name_(name), connected_flag(flag), prom_(), promise_set_(false) {
    }

    void on_success(const mqtt::token& tok) override {
        std::cout << name_ << " successful." << std::endl;
        if (name_ == "Connect") {
            connected_flag.store(true);
        }
        if (!promise_set_.exchange(true)) { // Only set if not already set
            prom_.set_value();
        }
        else {
            std::cerr << "Warning: " << name_ << " on_success called multiple times for token " << tok.get_message_id() << std::endl;
        }
    }

    void on_failure(const mqtt::token& tok) override {
        std::cerr << name_ << " failed. Error: " << tok.get_return_code() << std::endl;
        if (name_ == "Connect") {
            connected_flag.store(false);
        }
        if (!promise_set_.exchange(true)) { // Only set if not already set
            prom_.set_exception(std::make_exception_ptr(std::runtime_error("MQTT action failed")));
        }
        else {
            std::cerr << "Warning: " << name_ << " on_failure called multiple times for token " << tok.get_message_id() << std::endl;
        }
    }

    void wait() {
        prom_.get_future().wait();
    }
};

class mycallback;

class MqttHandler: public std::enable_shared_from_this<MqttHandler> {
public:
    MqttHandler(const std::string SERVER_ADDRESS, const std::string CLIENT_ID);

    void init();
    int connect();
    void subscribe(const std::string topic);
    void publish(const std::string topic, const std::string msg);
    void publish(const std::string topic, const void* data, size_t length);
    void disconnect();
    void processCommandResponse(const void* payload_ptr, size_t payload_len);
    void close();

private:
    mqtt::async_client cli;
    std::shared_ptr<mycallback> cb;
    myaction_listener connect_listener;
    myaction_listener subscribe_listener;
    std::atomic<bool> is_connected{ false };
    mqtt::connect_options connOpts;
};

class mycallback : public virtual mqtt::callback
{
public:
    mycallback(std::shared_ptr<MqttHandler> _mqttHandler) : cbMqttHandler(_mqttHandler) {}

    std::shared_ptr<std::ofstream> outputFileStream;
    const std::string outputFileName = "output.bin";

    void connection_lost(const std::string& cause) override {
        std::cout << "\nConnection lost";
        if (!cause.empty()) {
            std::cout << ": " << cause;
        }
        std::cout << std::endl;

        // reconnect
        // ...
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        //std::cout << "Delivery complete for token: " << (tok ? tok->get_message_id() : -1) << std::endl;
    }

    void message_arrived(mqtt::const_message_ptr msg) override {
        //std::cout << "Message arrived on topic: '" << msg->get_topic() << "' from " << msg->get_qos()
            //<< ": " << msg->to_string() << std::endl;

        std::string topic = msg->get_topic();
        if (topic == TOPIC_SUB_SENSORDATA) {
            const mqtt::binary_ref& payload_ref = msg->get_payload_ref();
            const void* payload_ptr = payload_ref.data();
            size_t payload_len = payload_ref.size();

            // Write file
            if (outputFileStream && outputFileStream->is_open()) {
                outputFileStream->write(reinterpret_cast<const char*>(payload_ptr), payload_len);
                // outputFileStream->flush(); // Consider flushing periodically for embedded systems

                //std::cout << "Data written to '" << outputFileName << "': " << payload_len << " bytes." << std::endl;
            }
            else {
                std::cerr << "Error: Output file is not open. Cannot write protocol data." << std::endl;
            }
        }
        else if (topic == TOPIC_SUB_PROTOCOLDATA) {
            const mqtt::binary_ref& payload_ref = msg->get_payload_ref();
            const void* payload_ptr = payload_ref.data();
            size_t payload_len = payload_ref.size();

            cbMqttHandler->processCommandResponse(payload_ptr, payload_len);
        }
    }

    bool initOutputFile() {
        outputFileStream = std::make_shared<std::ofstream>(outputFileName, std::ios::binary | std::ios::app);

        if (!outputFileStream->is_open()) {
            std::cerr << "Error: Unable to open output file '" << outputFileName << "'" << std::endl;
            return false;
        }
        else {
            std::cout << "Successfully opened output file: '" << outputFileName << "'" << std::endl;
            return true;
        }
    }

    void close() {
        if (outputFileStream && outputFileStream->is_open()) {
            outputFileStream->close();
            std::cout << "Output file '" << outputFileName << "' closed." << std::endl;
        }
    }

private:
    std::shared_ptr<MqttHandler> cbMqttHandler;
};

// ========================================================================================================
// MqttHandler.cpp
// ========================================================================================================

MqttHandler::MqttHandler(const std::string SERVER_ADDRESS, const std::string CLIENT_ID) :
    cli(SERVER_ADDRESS, CLIENT_ID),
    connect_listener("Connect", is_connected),
    subscribe_listener("Subscribe", is_connected),
    cb(nullptr)
{}

void MqttHandler::init() {
    cb = std::make_shared<mycallback>(this->shared_from_this());
    cli.set_callback(*cb); // Set the Paho client's callback
    if (!cb->initOutputFile()) {
        std::cerr << "Failed to initialize output file in MqttHandler setup_callback." << std::endl;
    }
    connOpts.set_clean_session(true);
    connOpts.set_keep_alive_interval(20);
    connOpts.set_automatic_reconnect(true);
}

int MqttHandler::connect() {
    std::cout << "Attempting to connect to MQTT broker at: " << SERVER_ADDRESS << std::endl;
    std::cout << "Client ID: " << CLIENT_ID << std::endl;

    std::cout << "Connecting..." << std::endl;
    cli.connect(connOpts, nullptr, connect_listener)->wait(); // Use listener to wait for connection success/failure

    if (!is_connected.load()) {
        std::cerr << "Failed to connect to MQTT broker." << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void MqttHandler::subscribe(const std::string topic) {
    std::cout << "Subscribing to topic: '" << topic << "' with QoS " << QOS << std::endl;
    cli.subscribe(topic, QOS, nullptr, subscribe_listener)->wait();
}

void MqttHandler::publish(const std::string topic, const std::string msg) {
    mqtt::message_ptr pubmsg = mqtt::make_message(topic, msg, QOS, TO_RETAIN);
    myaction_listener publish_listener("Publish", is_connected);

    try {
        //std::cout << "Publishing to topic: '" << topic << "': " << msg << std::endl;
        cli.publish(pubmsg, nullptr, publish_listener)->wait();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error publishing message: " << exc.what() << std::endl;
    }
}

void MqttHandler::publish(const std::string topic, const void* data, size_t length) {
    mqtt::message_ptr pubmsg = mqtt::make_message(topic, data, length, QOS, TO_RETAIN);
    myaction_listener publish_listener("Binary Publish", is_connected); // Use a distinct name for logging

    try {
        //std::cout << "Publishing " << length << " bytes to topic: '" << topic << "'" << std::endl;
        if (false) {
            // debug
            const uint8_t* byte_data = static_cast<const uint8_t*>(data);
            //std::cout << "  Data: [";
            //for (size_t i = 0; i < std::min(length, (size_t)10); ++i) { // Print up to 10 bytes
            //    std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte_data[i]) << " ";
            //}
            //if (length > 10) std::cout << "...";
            //std::cout << std::dec << "]" << std::endl;
        }

        //cli.publish(pubmsg, nullptr, publish_listener)->wait();
        cli.publish(pubmsg)->wait();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error publishing binary message: " << exc.what() << std::endl;
    }
}

void MqttHandler::disconnect() {
    std::cout << "\nDisconnecting..." << std::endl;
    cli.disconnect()->wait();
    std::cout << "Disconnected." << std::endl;
}

void MqttHandler::processCommandResponse(const void* payload_ptr, size_t payload_len) {
    std::cout << hexToString(payload_ptr, payload_len) << std::endl;

    const uint8_t* byte_data = static_cast<const uint8_t*>(payload_ptr);
    // do stuff...
}

void MqttHandler::close() {
    cb->close();
}

std::string hexToString(const void* data, size_t length) {
    std::ostringstream oss; // Use stringstream to build the string
    oss << "[";

    const uint8_t* byte_data = static_cast<const uint8_t*>(data); // Cast to uint8_t* for byte-level access

    for (size_t i = 0; i < length; ++i) {
        oss << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte_data[i]);
        if (i < length - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str(); // Get the built string from the stringstream
}


// ========================================================================================================
// main.cpp
// ========================================================================================================

// ============== COMMON USECASE ==============

void publish_task();
std::shared_ptr<MqttHandler> mqttHandler{ nullptr };
bool stop_publishing = false;

int mqttRun() {
    mqttHandler = std::make_shared<MqttHandler>(SERVER_ADDRESS, CLIENT_ID);
    mqttHandler->init();

    try {
        auto err = mqttHandler->connect();
        if (err == EXIT_FAILURE) return EXIT_FAILURE;

        mqttHandler->subscribe(TOPIC_SUB_SENSORDATA);
        mqttHandler->subscribe(TOPIC_SUB_PROTOCOLDATA);

        mqttHandler->publish("server/msg", "Hello, MQTT world!");
        mqttHandler->publish(TOPIC_PUB_PROTOCOLDATA, start_streaming_cmd.data(), start_streaming_cmd.size());
        mqttHandler->publish(TOPIC_PUB_PROTOCOLDATA, hexToString(start_streaming_cmd.data(), start_streaming_cmd.size()));
        std::this_thread::sleep_for(std::chrono::seconds(5));
        mqttHandler->publish(TOPIC_PUB_PROTOCOLDATA, stop_streaming_cmd.data(), stop_streaming_cmd.size());
        mqttHandler->publish(TOPIC_PUB_PROTOCOLDATA, hexToString(stop_streaming_cmd.data(), stop_streaming_cmd.size()));
        std::thread publisher_thread(publish_task);

        std::cout << "\nPublishing messages... Press ENTER to stop and disconnect." << std::endl;
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wait for Enter key

        // Set the flag to stop the publishing task
        stop_publishing = true;

        // Wait for the publishing thread to finish
        if (publisher_thread.joinable()) {
            publisher_thread.join();
        }

        mqttHandler->close();
        mqttHandler->disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Exception: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void publish_task() {
    std::cout << "\nStarting to publish messages. Press Ctrl+C to exit." << std::endl;

    int count = 0;
    while (!stop_publishing) {
        std::string payload = "Hello from Nexus client! Message number: " + std::to_string(++count);
        mqttHandler->publish(TOPIC_PUB_PROTOCOLDATA, payload);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "Publishing task finished." << std::endl;
}


// ============== HIGHDATARATE USECASE ==============

int mqttHighDataratePublisherRun() {
    mqttHandler = std::make_shared<MqttHandler>(SERVER_ADDRESS, CLIENT_ID);
    mqttHandler->init();

    try {
        auto err = mqttHandler->connect();
        if (err == EXIT_FAILURE) return EXIT_FAILURE;

        const uint8_t dummy_sensordata[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        };

        size_t len = sizeof(dummy_sensordata) / sizeof(dummy_sensordata[0]);

        auto start_time = std::chrono::high_resolution_clock::now();
        int count = 0;

        // Loop for 1 second
        while (std::chrono::high_resolution_clock::now() - start_time < std::chrono::seconds(30)) {
            mqttHandler->publish(TOPIC_SUB_SENSORDATA, dummy_sensordata, len);
            count++;
        }

        std::cout << "\nPublished data: " << count << " packets (" << len * count << " bytes)" << std::endl;

        mqttHandler->disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Exception: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int mqttHighDatarateSubscriberRun() {
    mqttHandler = std::make_shared<MqttHandler>(SERVER_ADDRESS, CLIENT_ID);
    mqttHandler->init();

    try {
        auto err = mqttHandler->connect();
        if (err == EXIT_FAILURE) return EXIT_FAILURE;

        mqttHandler->subscribe(TOPIC_SUB_SENSORDATA);

        std::cout << "Subscribing to " << TOPIC_SUB_SENSORDATA << " Press ENTER to stop and disconnect." << std::endl;
        // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Wait for Enter key

        mqttHandler->close();
        mqttHandler->disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT Exception: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// ---------

int main()
{
    fmt::print("Hello World!\n");
    return 0;
}