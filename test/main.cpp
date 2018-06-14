#include <exception>
#ifdef WIN32
#include <WinSock2.h>		// To stop windows.h including winsock.h
#endif
#include "UnitTest/UnitTest.h"
#include "TestCalculData.h"
#include "TestCalculator.h"
#include "TestMqttCalcul.h"

using namespace std;

int main()
{
    UnitTest unitTest;
    int ret = 0;


    try
    {
		unitTest.addTestClass(new TestCalculData());
		unitTest.addTestClass(new TestCalculator());
		unitTest.addTestClass(new TestMqttCalcul());
    }
    catch(const exception &e)
    {
        unitTest.displayError(e.what());
        ret = -1;
    }

    if(ret!=-1)
        if(!unitTest.run()) ret = 1;

	#if defined(_MSC_VER)
	cout << "Press any key..." << endl;
	getchar();
	#endif

	return ret;
}
