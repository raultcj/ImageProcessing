#include <iostream>

#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace std;

void connect(int movRequired) {
	int rate = 9600;
	String^ portName = "COM4";

	SerialPort^ arduino;
	arduino = gcnew SerialPort(portName, rate);

	try {
		arduino->Open();

		if (movRequired == 1) {
			arduino->WriteLine("1");
			cout << "Sent 1" << endl;
		}
		else if (movRequired == 0) {
			arduino->WriteLine("0");
			cout << "Sent 0" << endl;
		}
		else if (movRequired == 2) {
			arduino->WriteLine("2");
			cout << "Sent 2" << endl;
		}

		arduino->Close();
	}
	catch (IO::IOException^ e) {
		cout << "Port is not ready" << endl;
	}
	catch (ArgumentException^ e) {
		cout << "Incorrect port name" << endl;
	}
}