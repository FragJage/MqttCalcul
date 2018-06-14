#include "CacheManager.h"
#include "StringTools\StringTools.h"

using namespace std;

CacheManager::CacheManager(ICacheMessage* pCacheMsg) : m_CacheMsg(pCacheMsg)
{
}

CacheManager::~CacheManager()
{
}

bool CacheManager::AddSensor(const string& server, const string& topic, CalculData* pCalcul)
{
	map<string, MqttBridge>::iterator itMqtt;

	
	itMqtt = m_MqttClients.find(server);
	if (itMqtt == m_MqttClients.end())
	{
		try
		{
			m_MqttClients.emplace(piecewise_construct, forward_as_tuple(server), forward_as_tuple(server, m_CacheMsg));
		}
		catch (exception& e)
		{
			return false;
		}
		itMqtt = m_MqttClients.find(server);
	}

	itMqtt->second.Subscribe(topic);
	
	map<string, CacheValue>::iterator itCacheDevices;
	itCacheDevices = m_CacheDevices.find(server + ":" + topic);

	if (itCacheDevices == m_CacheDevices.end())
	{
		CacheValue tempDevice(pCalcul);
		m_CacheDevices.emplace(piecewise_construct, forward_as_tuple(server + ":" + topic), forward_as_tuple(tempDevice));
		return true;
	}

	if (itCacheDevices->second.Calculs.find(pCalcul) == itCacheDevices->second.Calculs.end())
	{
		itCacheDevices->second.Calculs.emplace(pCalcul);
	}
	return true;
}

bool CacheManager::UpdateValue(const string& key, const string& message)
{
	map<string, CacheValue>::iterator itCacheDevices = m_CacheDevices.find(key);
	string value = message;
	bool ret;

	if (itCacheDevices == m_CacheDevices.end()) return false;

	if (StringTools::IsEqualCaseInsensitive(message, "set")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "clear")) value = "0";
	else if (StringTools::IsEqualCaseInsensitive(message, "enable")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "disable")) value = "0";
	else if (StringTools::IsEqualCaseInsensitive(message, "yes")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "no")) value = "0";
	else if (StringTools::IsEqualCaseInsensitive(message, "high")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "low")) value = "0";
	else if (StringTools::IsEqualCaseInsensitive(message, "start")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "stop")) value = "0";
	else if (StringTools::IsEqualCaseInsensitive(message, "on")) value = "1";
	else if (StringTools::IsEqualCaseInsensitive(message, "off")) value = "0";

	ret = (!itCacheDevices->second.Readed) | (itCacheDevices->second.Value != value);

	itCacheDevices->second.Value = value;
	itCacheDevices->second.Readed = true;

	return ret;
}

const CacheValue& CacheManager::GetValue(const string& key)
{
	map<string, CacheValue>::iterator itCacheDevices = m_CacheDevices.find(key);
	if (itCacheDevices == m_CacheDevices.end()) return m_EmptyValue;
	return itCacheDevices->second;
}

const set<CalculData*>& CacheManager::GetCalculs(const string& key)
{
	map<string, CacheValue>::iterator itCacheDevices = m_CacheDevices.find(key);
	if (itCacheDevices == m_CacheDevices.end()) return m_EmptySet;
	return itCacheDevices->second.Calculs;
}
