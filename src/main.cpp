#include <iostream>
#include <exception>
#include "MqttCalcul.h"

using namespace std;

int main(int argc, char* argv[])
{
    int res = 0;

    try
    {
        MqttCalcul mqttCalcul;

        Service* pService = Service::Create("MqttCalcul", "Calculator for mqtt protocol", &mqttCalcul);
        res = pService->Start(argc, argv);
        Service::Destroy();
    }
    catch(const exception &e)
    {
        std::cout << e.what();
    }

	#if defined(_MSC_VER) && defined(DEBUG)
	cout << "Press any key..." << endl;
	getchar();
	#endif

    return res;
}

