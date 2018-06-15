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
	cout << "M1" << endl;
    UnitTest unitTest;
    int ret = 0;

    try
    {
		cout << "M2" << endl;
		unitTest.addTestClass(new TestCalculData());
		cout << "M3" << endl;
		unitTest.addTestClass(new TestCalculator());
		cout << "M4" << endl;
		unitTest.addTestClass(new TestMqttCalcul());
		cout << "M5" << endl;
    }
    catch(const exception &e)
    {
		cout << "M6" << endl;
        unitTest.displayError(e.what());
        ret = -1;
		cout << "M7" << endl;
    }

	cout << "M8" << endl;
    if(ret!=-1)
        if(!unitTest.run()) ret = 1;

	cout << "M9" << endl;
	#if defined(_MSC_VER)
	cout << "Press any key..." << endl;
	getchar();
	#endif

	return ret;
}
