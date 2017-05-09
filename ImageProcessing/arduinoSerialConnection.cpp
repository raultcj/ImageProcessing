#include <iostream>

#using <System.dll>

using namespace System;
using namespace System::IO::Ports;
using namespace std;

void connect(int posX) {
	int rate = 9600;
	String^ portName = "COM4";

	SerialPort^ arduino;
	arduino = gcnew SerialPort(portName, rate);

	try {
		arduino->Open();

		arduino->WriteLine(posX.ToString());

		cout << posX << endl;

		arduino->Close();
	}
	catch (IO::IOException^ e) {
		cout << "Port is not ready" << endl;
	}
	catch (ArgumentException^ e) {
		cout << "Incorrect port name" << endl;
	}
}