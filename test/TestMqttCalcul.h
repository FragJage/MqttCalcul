#include <thread>
#include <map>
#include <cassert>
#ifdef WIN32
#include <WinSock2.h>		// To stop windows.h including winsock.h
#endif
#include "Plateforms/Plateforms.h"
#include "UnitTest/UnitTest.h"
#include "MqttBase\MqttClient.h"
#include "../src/MqttCalcul.h"


class TestMqttCalcul : public TestClass<TestMqttCalcul>, public MqttClient::IMqttMessage
{
public:
	TestMqttCalcul();
	~TestMqttCalcul();
	static void ThreadStart(MqttCalcul* pMqttCalcul);
	void on_message(const std::string& topic, const std::string& message);

	bool Start();
	bool Calculation();
	bool Commands();
	bool Stop();

private:
	void waitMsg(size_t maxMsg, int maxTime);
	MqttCalcul mqttCalcul;
	MqttClient mqttClient;
	MqttClient mqttSender;
	std::map<std::string, std::string> m_Messages;
};
