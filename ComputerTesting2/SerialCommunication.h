#include <math.h>
#include <stdio.h>

using namespace System::IO::Ports;
using namespace System;

delegate void DataReceiveDelegate(int);

#pragma once
ref class SerialCommunication
{
public:
	SerialCommunication(DataReceiveDelegate^ callbackFunction);

	int Open();

	void Close();

	
void SerialCommunication::Threadstarter()
{
	int count = 0;
	System::TimeSpan delay = System::TimeSpan(1000);//100us
	DateTime start = DateTime::Now;	

	while(true)
	{
		double angle = 2.0 * 3.141592654 * (DateTime::Now - start).TotalMilliseconds / 60.0;
		double angle2 = 2.0 * 3.141592654 * (DateTime::Now - start).TotalMilliseconds / 100.0;
		int n = (int)(50 * sin(angle) + 50 * sin(angle2) + 120);

		this->_callbackFunction->Invoke(n);
		count++;
		System::Threading::Thread::Sleep(1);
	}
}

private:
	void DataReceivedHandler(Object^ sender, SerialDataReceivedEventArgs^ e);

	DataReceiveDelegate^ _callbackFunction;
	SerialPort^ _serialPort;
	FILE* fp;

};

