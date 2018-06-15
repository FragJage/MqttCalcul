#include "TestMqttCalcul.h"

using namespace std;

TestMqttCalcul::TestMqttCalcul() : TestClass("MqttCalcul", this)
{
cout << "N1" << endl;
	addTest("Start", &TestMqttCalcul::Start);
	addTest("Calculation", &TestMqttCalcul::Calculation);
	addTest("Commands", &TestMqttCalcul::Commands);
	addTest("Stop", &TestMqttCalcul::Stop);
cout << "N2" << endl;

	mqttClient.SetMessageCallback(this);
cout << "N3" << endl;
	mqttClient.Connect();
cout << "N4" << endl;
	mqttClient.Subscribe("calcul/#");
cout << "N5" << endl;
	mqttClient.SetMainTopic("calcul");
cout << "N6" << endl;

	mqttSender.Connect();
cout << "N7" << endl;
	mqttSender.SetMainTopic("owfs");
cout << "N8" << endl;
}

TestMqttCalcul::~TestMqttCalcul()
{
	mqttSender.Disconnect();
	mqttClient.Disconnect();
}

void TestMqttCalcul::ThreadStart(MqttCalcul* pMqttDev)
{
	char exeName[] = "test";
	char consArg[] = "-console";
	char confArg[] = "--configfile";
	char confName[] = "MqttCalcul.conf";
	char levelArg[] = "--loglevel";
	char levelName[] = "2";
	char destArg[] = "--logdestination";
	char destName[] = "cerr";
	char* argv[8];

	argv[0] = exeName;
	argv[1] = consArg;
	argv[2] = confArg;
	argv[3] = confName;
	argv[4] = levelArg;
	argv[5] = levelName;
	argv[6] = destArg;
	argv[7] = destName;

	Service* pService = Service::Create("MqttCalcul", "Calculator for mqtt protocol", pMqttDev);
	pService->Start(8, argv);
}

void TestMqttCalcul::on_message(const string& topic, const string& message)
{
	//cout << "###" << topic << " : " << message << endl;
	m_Messages[topic] = message;
}

void TestMqttCalcul::waitMsg(size_t maxMsg, int maxTime)
{
	int time = 0;
	bool stop = false;

	while(!stop)
	{
		if(m_Messages.size() >= maxMsg) stop = true;
		if (time > maxTime) stop = true;
		time += 50;
		Plateforms::delay(50);
	}
}

bool TestMqttCalcul::Start()
{
	thread integrationTest(ThreadStart, &mqttCalcul);
	integrationTest.detach();

	waitMsg(1, 5000);
	map<string, string>::iterator it;
	it = m_Messages.find("calcul/FixedSix");
	assert(m_Messages.end() != it);
	assert("6" == it->second);
	m_Messages.clear();

	return true;
}

bool TestMqttCalcul::Calculation()
{
	mqttSender.Publish("ESPool", "255");

	waitMsg(1, 1000);
	map<string, string>::iterator it;
	it = m_Messages.find("calcul/FiltrationPiscine");
	assert(m_Messages.end() != it);
	assert("ON" == it->second);
	m_Messages.clear();

	return true;
}

bool TestMqttCalcul::Commands()
{
	map<string, string>::iterator it;

	mqttClient.Publish("command/FiltrationPiscine", "REQUEST");
	waitMsg(2, 500);
	it = m_Messages.find("calcul/FiltrationPiscine");
	assert(m_Messages.end() != it);
	assert("ON" == it->second);
	m_Messages.clear();

	mqttClient.Publish("command", "REQUEST");
	waitMsg(3, 500);
	it = m_Messages.find("calcul/FiltrationPiscine");
	assert(m_Messages.end() != it);
	assert("ON" == it->second);
	it = m_Messages.find("calcul/FixedSix");
	assert(m_Messages.end() != it);
	assert("6" == it->second);
	m_Messages.clear();

	return true;
}

bool TestMqttCalcul::Stop()
{
	Service::Get()->ChangeStatus(Service::StatusKind::PAUSE);
	Plateforms::delay(50);
	Service::Get()->ChangeStatus(Service::StatusKind::START);
	Plateforms::delay(50);
	Service::Get()->ChangeStatus(Service::StatusKind::STOP);
	Plateforms::delay(100);

	return true;
}
