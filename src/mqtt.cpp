#include "../include/mqtt.h"
#include <iostream>
#include <chrono>
#include <thread>

namespace mqtt
{
	const std::string message::EMPTY_STR;
}

class callback : public virtual mqtt::callback
{
public:
	void message_arrived(mqtt::const_message_ptr msg) override
	{
		fmt::print("\nReceived message:\n\tTopic: {}\n\tPayload: {}\n",
				   msg->get_topic(), msg->get_payload_str());
	}

	void connection_lost(const std::string &cause) override
	{
		fmt::print("\nConnection lost: {}\n", cause);
	}
};

void run_mqtt()
{
	const std::string SERVER_ADDRESS = "test.mosquitto.org";
	const std::string CLIENT_ID = "my_mqtt_tester";
	const std::string PUB_TOPIC = "royyan_topic/test";
	const std::string SUB_TOPIC = "royyan_topic/test_sub";

	// Create MQTT client
	mqtt::async_client mqtt_client(SERVER_ADDRESS, CLIENT_ID);
	fmt::print("MQTT client instance created: Address={}, ClientID={}\n",
			   SERVER_ADDRESS, CLIENT_ID);

	// Set callback
	callback cb;
	mqtt_client.set_callback(cb);

	// Create connection options
	mqtt::connect_options conn_opts;
	conn_opts.set_keep_alive_interval(20);
	conn_opts.set_clean_session(true);

	try
	{
		// Connect to the broker
		fmt::print("Connecting to the MQTT broker...\n");
		mqtt_client.connect(conn_opts)->wait();
		fmt::print("Connected to the MQTT broker\n");

		// Subscribe to topic
		fmt::print("Subscribing to topic: {}\n", SUB_TOPIC);
		mqtt_client.subscribe(SUB_TOPIC, 1)->wait();
		fmt::print("Subscribed to topic: {}\n", SUB_TOPIC);

		// Create and publish message
		mqtt::message_ptr msg = mqtt::make_message(PUB_TOPIC, "Hello MQTT linker!");
		fmt::print("Publishing message:\n\tTopic: {}\n\tPayload: {}\n",
				   msg->get_topic(), msg->get_payload_str());
		mqtt_client.publish(msg)->wait();
		fmt::print("Message published\n");

		// Set up timer for 3 seconds
		auto start = std::chrono::steady_clock::now();
		fmt::print("\nWaiting for incoming messages for 3 seconds...\n");

		// Keep running for 3 seconds
		while (std::chrono::steady_clock::now() - start < std::chrono::seconds(3))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Disconnect
		fmt::print("\n3 seconds elapsed. Disconnecting from the MQTT broker...\n");
		mqtt_client.disconnect()->wait();
		fmt::print("Disconnected\n");
	}
	catch (const mqtt::exception &exc)
	{
		fmt::print(stderr, "MQTT error: {}\n", exc.what());
	}
}