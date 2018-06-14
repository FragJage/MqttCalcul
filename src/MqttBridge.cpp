#include "MqttBridge.h"

using namespace std;

MqttBridge::MqttBridge(const string& server, ICacheMessage* callback) : m_Server(server), m_Callback(callback)
{
	SetServer(m_Server, "");
	Connect();
}

MqttBridge::~MqttBridge()
{
	Disconnect();
}

void MqttBridge::Subscribe(const string& topic)
{
	if (m_Topics.count(topic) > 0) return;
	m_Topics.emplace(topic);
	MqttBase::Subscribe(topic);
}

void MqttBridge::on_message(const string& topic, const string& message)
{
	m_Callback->on_cache(m_Server, topic, message);
	return;
}