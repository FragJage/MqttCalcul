#include <cassert>
#include "UnitTest/UnitTest.h"
#include "../src/CalculData.h"

using namespace std;

class TestCalculData : public TestClass<TestCalculData>
{
public:
	TestCalculData();
    ~TestCalculData();

    bool Constructor();
	bool Type();
	bool Devices();
	bool Value();
};
