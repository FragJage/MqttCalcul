#ifndef ICACHEMESSAGE_H
#define ICACHEMESSAGE_H

#include <string>

class ICacheMessage
{
public:
	virtual ~ICacheMessage() {};
	virtual void on_cache(const std::string& server, const std::string& topic, const std::string& message) = 0;
};

#endif // ICACHEMESSAGE_H
