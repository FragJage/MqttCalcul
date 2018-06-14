#ifndef MQTTBRIDGE_H
#define MQTTBRIDGE_H

#include <string>
#include <set>
#include "MqttBase/MqttBase.h"
#include "ICacheMessage.h"

class MqttBridge : public MqttBase
{
    public:
		MqttBridge(const std::string& server, ICacheMessage* callback);
        ~MqttBridge();
		void Subscribe(const std::string& topic);
		void on_message(const std::string& topic, const std::string& message);

	private:
		std::string m_Server;
		ICacheMessage* m_Callback;
		std::set<std::string> m_Topics;
};

#endif // MQTTBRIDGE_H
