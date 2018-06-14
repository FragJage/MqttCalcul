#include "TestCalculator.h"

using namespace std;

TestCalculator::TestCalculator() : TestClass("TestCalculator", this)
{
	addTest("Addition", &TestCalculator::Addition);
	addTest("Substraction", &TestCalculator::Substraction);
	addTest("Multiplication", &TestCalculator::Multiplication);
	addTest("Division", &TestCalculator::Division);
	addTest("BinaryAnd", &TestCalculator::BinaryAnd);
	addTest("BinaryOr", &TestCalculator::BinaryOr);
	addTest("GreaterThan", &TestCalculator::GreaterThan);
	addTest("LessThan", &TestCalculator::LessThan);
	addTest("Priority", &TestCalculator::Priority);
	addTest("Brackets", &TestCalculator::Brackets);
}

TestCalculator::~TestCalculator()
{
}

bool TestCalculator::Addition()
{
    string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6";
	values["localhost:/topic/value2"] = "10.5";
	values["localhost:/topic/value3"] = "-3";

	CalculData FixedInt("FixedInt", "Num", "5+3");
	result = Calculator::Evaluate(&FixedInt, values);
    assert("8"== result);

	CalculData FixedFloat("FixedFloat", "Num", "5.9+8.2");
	result = Calculator::Evaluate(&FixedFloat, values);
	assert("14.1" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}+{localhost:/topic/value2}+{localhost:/topic/value3}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("13.5" == result);

	CalculData Mixed1("Mixed1", "Num", "{localhost:/topic/value1}+11.3");
	result = Calculator::Evaluate(&Mixed1, values);
	assert("17.3" == result);

	CalculData Mixed2("Mixed2", "Num", "15.6+{localhost:/topic/value2}");
	result = Calculator::Evaluate(&Mixed2, values);
	assert("26.1" == result);

	CalculData Switch("Switch", "Switch", "2+{localhost:/topic/value3}");
	result = Calculator::Evaluate(&Switch, values);
	assert("-1" == result);

	return true;
}

bool TestCalculator::Substraction()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6";
	values["localhost:/topic/value2"] = "10.5";
	values["localhost:/topic/value3"] = "-3";

	CalculData FixedInt("FixedInt", "Num", "5-3");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("2" == result);

	CalculData FixedFloat("FixedFloat", "Num", "5.9-8.2");
	result = Calculator::Evaluate(&FixedFloat, values);
	assert("-2.3" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}-{localhost:/topic/value2}-{localhost:/topic/value3}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("-1.5" == result);

	CalculData Switch("Switch", "Switch", "2-{localhost:/topic/value3}");
	result = Calculator::Evaluate(&Switch, values);
	assert("5" == result);

	return true;
}

bool TestCalculator::Multiplication()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6.1";
	values["localhost:/topic/value2"] = "2.5";

	CalculData FixedInt("FixedInt", "Num", "5*3");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("15" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}*{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("15.25" == result);

	CalculData Switch("Switch", "Switch", "2*{localhost:/topic/value2}");
	result = Calculator::Evaluate(&Switch, values);
	assert("5" == result);

	return true;
}

bool TestCalculator::Division()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6.1";
	values["localhost:/topic/value2"] = "2.5";

	CalculData FixedInt("FixedInt", "Num", "15/5");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("3" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}/{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("2.44" == result);

	CalculData Switch("Switch", "Switch", "{localhost:/topic/value2}/2");
	result = Calculator::Evaluate(&Switch, values);
	assert("1.25" == result);

	return true;
}

bool TestCalculator::BinaryAnd()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6";
	values["localhost:/topic/value2"] = "3";

	CalculData FixedInt("FixedInt", "Num", "5&2");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("0" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}&{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("2" == result);

	return true;
}

bool TestCalculator::BinaryOr()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "6";
	values["localhost:/topic/value2"] = "3";

	CalculData FixedInt("FixedInt", "Num", "5|2");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("7" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}|{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("7" == result);

	return true;
}

bool TestCalculator::GreaterThan()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "3";
	values["localhost:/topic/value2"] = "6";

	CalculData FixedInt("FixedInt", "Num", "5>2");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("1" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}>{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("0" == result);

	return true;
}

bool TestCalculator::LessThan()
{
	string result;

	map<string, string> values;
	values["localhost:/topic/value1"] = "3";
	values["localhost:/topic/value2"] = "6";

	CalculData FixedInt("FixedInt", "Num", "5<2");
	result = Calculator::Evaluate(&FixedInt, values);
	assert("0" == result);

	CalculData WithVar("WithVar", "Num", "{localhost:/topic/value1}<{localhost:/topic/value2}");
	result = Calculator::Evaluate(&WithVar, values);
	assert("1" == result);

	return true;
}

bool TestCalculator::Priority()
{
	string result;
	map<string, string> values;

	CalculData AddMul1("AddMul1", "Num", "5+2*3");
	result = Calculator::Evaluate(&AddMul1, values);
	assert("11" == result);

	CalculData AddMul2("AddMul2", "Num", "5*2+3");
	result = Calculator::Evaluate(&AddMul2, values);
	assert("13" == result);

	CalculData SubDiv1("SubDiv1", "Num", "5-4/2");
	result = Calculator::Evaluate(&SubDiv1, values);
	assert("3" == result);

	CalculData SubDiv2("SubDiv2", "Num", "6/2+3");
	result = Calculator::Evaluate(&SubDiv2, values);
	assert("6" == result);

	CalculData MulAnd("MulAnd", "Num", "7*5&3");
	result = Calculator::Evaluate(&MulAnd, values);
	assert("7" == result);

	CalculData DivOr("DivOr", "Num", "8/5|3");
	result = Calculator::Evaluate(&DivOr, values);
	assert("1.14286" == result);

	CalculData Comp("Comp", "Num", "5*3-4/2+15/3-3*3");
	result = Calculator::Evaluate(&Comp, values);
	assert("9" == result);

	return true;
}

bool TestCalculator::Brackets()
{
	string result;
	map<string, string> values;

	CalculData AddMul1("AddMul1", "Num", "(5+2)*3");
	result = Calculator::Evaluate(&AddMul1, values);
	assert("21" == result);

	CalculData AddMul2("AddMul2", "Num", "5*(2+3)");
	result = Calculator::Evaluate(&AddMul2, values);
	assert("25" == result);

	CalculData SubDiv1("SubDiv1", "Num", "(5-4)/2");
	result = Calculator::Evaluate(&SubDiv1, values);
	assert("0.5" == result);

	CalculData SubDiv2("SubDiv2", "Num", "6/(2+3)");
	result = Calculator::Evaluate(&SubDiv2, values);
	assert("1.2" == result);
	return true;
}
