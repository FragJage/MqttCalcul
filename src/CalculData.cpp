#include <stdexcept>
#include "CalculData.h"
#include "StringTools\StringTools.h"

using namespace std;

CalculData::CalculData(string name, string type, string formula) : m_Name(name), m_Formula(formula), m_IsSet(false), m_Value("")
{
    m_Type = ToCalculType(type);
    FindDevices();
}

CalculData::~CalculData()
{
}

bool CalculData::IsNewValue(std::string value)
{
	if (m_Type == CalculType::Switch)
	{
		if (atoi(value.c_str()) > 0)
			value = "ON";
		else
			value = "OFF";
	}

	bool isNew = ((m_IsSet == false) | (m_Value != value));

	m_IsSet = true;
	m_Value = value;

	return isNew;
}

CalculData::CalculType CalculData::ToCalculType(string type)
{
    if(StringTools::IsEqualCaseInsensitive(type, "NUM")) return CalculType::Num;
    if(StringTools::IsEqualCaseInsensitive(type, "SWITCH")) return CalculType::Switch;
    throw logic_error("Invalid CalculType, waiting type 'num' or 'switch'");
}

void CalculData::FindDevices()
{
    string device, server, topic;
    size_t iStr;
    size_t debDevice, finDevice;


    finDevice = 0;
    do
    {
        debDevice = m_Formula.find("{", finDevice);
        if(debDevice==string::npos) break;

        finDevice = m_Formula.find("}", debDevice+1);
        if(finDevice==string::npos)
            throw logic_error("Invalid device, don't find end mark with '}'");

        device = m_Formula.substr(debDevice+1, finDevice-debDevice-1);
        iStr = device.rfind(":");
        if(iStr==string::npos)
            throw logic_error("Device without server (don't find ':')");

        server = device.substr(0, iStr);
        topic = device.substr(iStr+1);
        m_Devices.emplace_back(server, topic, debDevice, finDevice);
        debDevice = finDevice+1;
    } while(debDevice!=string::npos);
}

