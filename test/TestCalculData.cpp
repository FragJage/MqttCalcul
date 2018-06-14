#include "TestCalculData.h"

using namespace std;

TestCalculData::TestCalculData() : TestClass("TestCalculData", this)
{
	addTest("Constructor", &TestCalculData::Constructor);
	addTest("Type", &TestCalculData::Type);
	addTest("Devices", &TestCalculData::Devices);
	addTest("Value", &TestCalculData::Value);
}

TestCalculData::~TestCalculData()
{
}

bool TestCalculData::Constructor()
{
	CalculData myCalcul("myCalcul", "Num", "3+2*8/5");
	string result;

	result = myCalcul.GetName();
	assert("myCalcul" == result);

	result = myCalcul.GetFormula();
	assert("3+2*8/5" == result);

	return true;
}

bool TestCalculData::Type()
{
	CalculData myCalcul("myCalcul", "Num", "3+2*8/5");
	CalculData mySwitch("mySwitch", "Switch", "1|2");
	CalculData::CalculType myType;
	bool bCatch = false;

	myType = myCalcul.GetType();
	assert(CalculData::CalculType::Num == myType);

	myType = mySwitch.GetType();
	assert(CalculData::CalculType::Switch == myType);

	try
	{
		CalculData myError("myError", "XXXXX", "1+2");
	}
	catch (logic_error e)
	{
		string msg = e.what();
		assert("Invalid CalculType, waiting type 'num' or 'switch'" == msg);
		bCatch = true;
	}

	assert(true == bCatch);

	return true;
}

bool TestCalculData::Devices()
{
	CalculData myCalcul("myCalcul", "Num", "{tcp://localhost:1883:topic/value1}+{ssl://mqttserver:443:topic/value2}");
	bool bCatch;

	vector<CalculData::Device> devices = myCalcul.GetDevices();
	assert(2 == devices.size());
	assert("tcp://localhost:1883" == devices[0].Server);
	assert("topic/value1" == devices[0].Topic);
	assert("ssl://mqttserver:443" == devices[1].Server);
	assert("topic/value2" == devices[1].Topic);

	bCatch = false;
	try
	{
		CalculData myError1("myError1", "NUM", "{localhost:/topic/value1*1.5");
	}
	catch (logic_error e)
	{
		string msg = e.what();
		assert("Invalid device, don't find end mark with '}'" == msg);
		bCatch = true;
	}
	assert(true == bCatch);

	bCatch = false;
	try
	{
		CalculData myError2("myError2", "NUM", "{local/topic/value1}*1.5");
	}
	catch (logic_error e)
	{
		string msg = e.what();
		assert("Device without server (don't find ':')" == msg);
		bCatch = true;
	}
	assert(true == bCatch);

	return true;
}

bool TestCalculData::Value()
{
	CalculData myCalcul("myCalcul", "Num", "3+2*8/5");
	CalculData mySwitch("mySwitch", "Switch", "1|2");
	bool bResult;
	string Result;

	bResult = myCalcul.IsSet();
	assert(false == bResult);

	bResult = myCalcul.IsNewValue("215.6");
	assert(true == bResult);

	bResult = myCalcul.IsSet();
	assert(true == bResult);

	bResult = myCalcul.IsNewValue("215.6");
	assert(false == bResult);

	Result = myCalcul.GetValue();
	assert("215.6" == Result);

	mySwitch.IsNewValue("5");
	Result = mySwitch.GetValue();
	assert("ON" == Result);

	mySwitch.IsNewValue("-2");
	Result = mySwitch.GetValue();
	assert("OFF" == Result);

	return true;
}