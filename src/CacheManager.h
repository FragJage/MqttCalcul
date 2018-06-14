#ifndef CACHEMANAGER_H
#define CACHEMANAGER_H

#include <string>
#include <map>
#include <set>
#include "SimpleLog/SimpleLog.h"
#include "ICacheMessage.h"
#include "CalculData.h"
#include "MqttBridge.h"

class CacheValue
{
public:
	CacheValue() : Readed(false), Value("") {};
	CacheValue(CalculData* pCalcul) : Readed(false), Value("") { Calculs.emplace(pCalcul); };
	~CacheValue() {};

	bool Readed;
	std::string Value;
	std::set<CalculData*> Calculs;
};

class CacheManager
{
    public:
        CacheManager(ICacheMessage* pCacheMsg);
        ~CacheManager();

		bool AddSensor(const std::string& server, const std::string& topic, CalculData* pCalcul);
		bool UpdateValue(const std::string& key, const std::string& value);
		const CacheValue& GetValue(const std::string& key);
		const std::set<CalculData*>& GetCalculs(const std::string& key);

	private :
		std::map<std::string, MqttBridge> m_MqttClients;
		std::map<std::string, CacheValue> m_CacheDevices;
		std::set<CalculData*> m_EmptySet;
		CacheValue m_EmptyValue;
		ICacheMessage* m_CacheMsg;
};

#endif // CACHEMANAGER_H
