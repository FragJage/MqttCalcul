#include<iostream>
#ifdef WIN32
#include <WinSock2.h>		// To stop windows.h including winsock.h
#endif
#include "Plateforms/Plateforms.h"
#include "MqttCalcul.h"
#include "Calculator.h"

#include "MqttBase/MqttClient.h"


using namespace std;

MqttCalcul::MqttCalcul() : MqttDaemon("calcul", ".MqttCalcul"), m_CacheManager(this)
{
}

MqttCalcul::~MqttCalcul()
{
}

void MqttCalcul::DaemonConfigure(SimpleIni& iniFile)
{
    LOG_ENTER;
cout << "MqttCalcul Configure start";
	for (SimpleIni::SectionIterator itSection = iniFile.beginSection(); itSection != iniFile.endSection(); ++itSection)
	{
		if ((*itSection) == "mqtt") continue;
		if ((*itSection) == "log") continue;

        string name    = (*itSection);
        string type    = iniFile.GetValue(name, "type", "");
        string formula = iniFile.GetValue(name, "formula", "");
cout << ".";
        LOG_VERBOSE(m_Log) << "Create " << name <<"("<< type <<") = "<< formula;
        try
        {
            m_Calculs.emplace_back(name, type, formula);
        }
        catch(logic_error& e)
        {
            LOG_WARNING(m_Log) << "Unable to create " << name << " : " << e.what();
			continue;
        }
    }

	cout << endl << "MqttCalcul Configure before subcribe" << endl;
	for (vector<CalculData>::iterator it = m_Calculs.begin(); it != m_Calculs.end(); ++it)
	{
		CalculData* pCalculData = &(*it);
		vector<CalculData::Device> devices = pCalculData->GetDevices();
		for (auto itdev = devices.cbegin(); itdev != devices.cend(); ++itdev)
		{
cout << ".";
			LOG_VERBOSE(m_Log) << "Subcribe to server " << itdev->Server << ", topic " << itdev->Topic;
			if(!m_CacheManager.AddSensor(itdev->Server, itdev->Topic, pCalculData))
				LOG_WARNING(m_Log) << "Unable to find server " << itdev->Server << ", topic " << itdev->Topic;
		}
	}


cout << endl << "MqttCalcul Configure end" << endl;
	LOG_EXIT_OK;
}

void MqttCalcul::MessageForService(const string& msg)
{
	if (msg != "REQUEST")
	{
		LOG_WARNING(m_Log) << "Unknown command for service " << msg;
		return;
	}

	lock_guard<mutex> lock(m_MqttQueueAccess);
	for (auto it = m_Calculs.begin(); it != m_Calculs.end(); ++it)
	{
		if (!it->IsSet()) continue;
		m_MqttQueue.emplace(it->GetName(), it->GetValue());
	}
	m_MqttQueueCond.notify_one();
	return;
}

void MqttCalcul::MessageForDevice(const string& device, const string& msg)
{
	vector<CalculData>::iterator it;

	for (it = m_Calculs.begin(); it != m_Calculs.end(); ++it)
	{
		if (it->GetName() == device) break;
	}

	if (it == m_Calculs.end())
	{
		LOG_WARNING(m_Log) << "Unknown device " << device;
		return;
	}

	if (msg == "REQUEST")
	{
		if (!it->IsSet())
		{
			LOG_WARNING(m_Log) << "Request value for " << device << " but is not already set.";
			return;
		}

		lock_guard<mutex> lock(m_MqttQueueAccess);
		m_MqttQueue.emplace(it->GetName(), it->GetValue());
		m_MqttQueueCond.notify_one();
		return;
	}

	LOG_WARNING(m_Log) << "Unknown command " << msg << " for device " << device;
	return;
}

void MqttCalcul::on_cache(const string& server, const string& topic, const string& message)
{
	LOG_VERBOSE(m_Log) << "Mqtt receive " << server << ":/" << topic << " = " << message;

	string key = server + ":" + topic;
	if(!m_CacheManager.UpdateValue(key, message)) return;

	set<CalculData*> calculs = m_CacheManager.GetCalculs(key);

	vector<CalculData::Device> devices;
	map<string, string> values;
	CacheValue cacheValue;
	bool somethingToSend = false;


	for (auto itCalculs = calculs.begin(); itCalculs != calculs.end(); ++itCalculs)
	{
		devices = (*itCalculs)->GetDevices();
		bool allValues = true;
		values.clear();
		for (auto itDevices = devices.begin(); itDevices != devices.end(); ++itDevices)
		{
			key = itDevices->Server + ":" + itDevices->Topic;
			cacheValue = m_CacheManager.GetValue(key);
			if (cacheValue.Readed == false) { allValues = false;  break; }
			values[key] = cacheValue.Value;
		}
		if (allValues)
		{
			string value = Calculator::Evaluate(*itCalculs, values);
			if ((*itCalculs)->IsNewValue(value))
			{
				LOG_VERBOSE(m_Log) << "Mqtt send calcul " << (*itCalculs)->GetName() << " = " << (*itCalculs)->GetValue();
				lock_guard<mutex> lock(m_MqttQueueAccess);
				m_MqttQueue.emplace((*itCalculs)->GetName(), (*itCalculs)->GetValue());
				somethingToSend = true;
			}
		}
		else
		{
			LOG_INFO(m_Log) << "Unable to evaluate " << (*itCalculs)->GetName() << ", all devices are not readed";
		}
	}

	if(somethingToSend) m_MqttQueueCond.notify_one();

	return;
}

void MqttCalcul::on_message(const string& topic, const string& message)
{
	LOG_VERBOSE(m_Log) << "Mqtt receive " << topic << " : " << message;

	string mainTopic = GetMainTopic();
	if (topic.substr(0, mainTopic.length()) != mainTopic)
	{
		LOG_WARNING(m_Log) << "Not for me (" << mainTopic << ")";
		return;
	}

	if (topic.substr(mainTopic.length(), 7) != "command")
	{
		LOG_WARNING(m_Log) << "Not a command (waiting " << mainTopic + "command" << ")";
		return;
	}

	if (topic.length() == mainTopic.length() + 7) return MessageForService(message);

	string device = topic.substr(mainTopic.length() + 8);
	return MessageForDevice(device, message);
}

int MqttCalcul::DaemonLoop(int argc, char* argv[])
{
	LOG_ENTER;
cout << "MqttCalcul Loop start" << endl;

	Subscribe(GetMainTopic() + "command/#");
	LOG_VERBOSE(m_Log) << "Subscript to : " << GetMainTopic() + "command/#";

cout << "MqttCalcul Loop before initial sending";
	for (vector<CalculData>::iterator it = m_Calculs.begin(); it != m_Calculs.end(); ++it)
	{
		CalculData* pCalculData = &(*it);
		vector<CalculData::Device> devices = pCalculData->GetDevices();
		if (devices.size() != 0) continue;

		map<string, string> values;
		string value = Calculator::Evaluate(pCalculData, values);
		if (pCalculData->IsNewValue(value))
		{
cout << ".";
			LOG_VERBOSE(m_Log) << "Mqtt send calcul " << pCalculData->GetName() << " = " << pCalculData->GetValue();
			Publish(pCalculData->GetName(), pCalculData->GetValue());
		}
	}
cout << endl;
	bool bStop = false;
	bool bPause = false;
	while(!bStop)
    {
cout << "MqttCalcul Loop before wait" << endl;
		int cond = Service::Get()->Wait({ m_MqttQueueCond });
cout << "MqttCalcul Loop after wait : " << cond << endl;
		if (cond == Service::STATUS_CHANGED)
		{
			switch (Service::Get()->GetStatus())
			{
			case Service::StatusKind::PAUSE:
				bPause = true;
				break;
			case Service::StatusKind::START:
				bPause = false;
				cond = 1;
				break;
			case Service::StatusKind::STOP:
				bStop = true;
				break;
			}
		}
		if((!bPause)&&(cond == 1))
		{
			lock_guard<mutex> lock(m_MqttQueueAccess);
cout << "MqttCalcul Loop publish " << m_MqttQueue.size() << " ";
			while (!m_MqttQueue.empty())
			{
cout << ".";
				MqttQueue& mqttQueue = m_MqttQueue.front();
				Publish(mqttQueue.Topic, mqttQueue.Message);
				m_MqttQueue.pop();
			}
cout << "MqttCalcul Loop publish End" << endl;
		}
    }

	LOG_EXIT_OK;
    return 0;
}
