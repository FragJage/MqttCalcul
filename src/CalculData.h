#ifndef CALCULDATA_H
#define CALCULDATA_H

#include <string>
#include <vector>

class CalculData
{
    public:
        enum CalculType {Switch, Num};
        struct Device
        {
			Device(const std::string& server, const std::string& topic, size_t start, size_t end) : Server(server), Topic(topic), Start(start), End(end) {};
            std::string Server;
            std::string Topic;
            size_t Start;
            size_t End;
        };
        CalculData(const std::string& name, const std::string& type, const std::string& formula);
        ~CalculData();

        std::string GetName() { return m_Name; }
        CalculType GetType() { return m_Type; }
        std::string GetFormula() { return m_Formula; }
		bool IsSet() { return m_IsSet; }
		std::string GetValue() { return m_Value; }
		const std::vector<Device>& GetDevices() { return m_Devices; }
		bool IsNewValue(const std::string& value);

    protected:

    private:
        std::string m_Name;
        CalculType m_Type;
        std::string m_Formula;
        std::vector<Device> m_Devices;
		bool m_IsSet;
		std::string m_Value;

        CalculType ToCalculType(std::string type);
        void FindDevices();
};

#endif // CALCULDATA_H
