#ifndef MQTTCALCUL_H
#define MQTTCALCUL_H

#include <mutex>
#include <queue>
#include "MqttBase/MqttDaemon.h"
#include "CalculData.h"
#include "CacheManager.h"

struct MqttQueue
{
	MqttQueue(std::string topic, std::string msg) : Topic(topic), Message(msg) {};
	std::string Topic;
	std::string Message;
};

class MqttCalcul : public MqttDaemon, ICacheMessage
{
    public:
        MqttCalcul();
        ~MqttCalcul();

		int DaemonLoop(int argc, char* argv[]);
        void on_message(const std::string& topic, const std::string& message);
		void on_cache(const std::string& server, const std::string& topic, const std::string& message);

    private:
		void DaemonConfigure(SimpleIni& iniFile);
		void MessageForService(const std::string& msg);
		void MessageForDevice(const std::string& device, const std::string& msg);
		std::vector<CalculData> m_Calculs;
		CacheManager m_CacheManager;
		std::mutex m_MqttQueueAccess;
		ServiceConditionVariable m_MqttQueueCond;
		std::queue<MqttQueue> m_MqttQueue;
};
#endif // MQTTCALCUL_H
