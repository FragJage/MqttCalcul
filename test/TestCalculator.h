#include <cassert>
#include "UnitTest/UnitTest.h"
#include "../src/Calculator.h"

using namespace std;

class TestCalculator : public TestClass<TestCalculator>
{
public:
	TestCalculator();
    ~TestCalculator();

    bool Addition();
    bool Substraction();
    bool Multiplication();
	bool Division();
	bool BinaryAnd();
	bool BinaryOr();
	bool GreaterThan();
	bool LessThan();
	bool Priority();
	bool Brackets();
};
