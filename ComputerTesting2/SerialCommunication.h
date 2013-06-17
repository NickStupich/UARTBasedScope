#include <math.h>
#include <stdio.h>

enum SampleRate{
	SPS_32k = 0x0,
	SPS_16k = 0x1,
	SPS_8k = 0x2,
	SPS_4k = 0x3,
	SPS_2k = 0x4,
	SPS_1k = 0x5,
	SPS_500 = 0x6
};

using namespace System::IO::Ports;
using namespace System;

delegate void DataReceiveDelegate(double);

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
		double angle = 2.0 * 3.141592654 * (((double) count) / 100.0);
		double angle2 = 2.0 * 3.141592654 * (((double) count) / 60.0);
		double n = (50 * sin(angle) + 80 * sin(angle2));

		this->_callbackFunction->Invoke(n);
		count++;
		System::Threading::Thread::Sleep(1);
		//System::Threading::Thread::Sleep(0.5);
	}
}

private:
	void DataReceivedHandler(Object^ sender, SerialDataReceivedEventArgs^ e);

	DataReceiveDelegate^ _callbackFunction;
	SerialPort^ _serialPort;
	FILE* fp;

};

