#include "StdAfx.h"
#include "SerialCommunication.h"

#define useFakeData	0

typedef union{
	unsigned char us;
	char si;
} unToSigned;

SerialCommunication::SerialCommunication(DataReceiveDelegate^ callbackFunction)
{
	this->_callbackFunction = callbackFunction;
	this->_serialPort = gcnew SerialPort();
	this->_serialPort->PortName = "COM12";
	this->_serialPort->BaudRate = 115200;
	this->_serialPort->Parity = Parity::None;
	this->_serialPort->DataBits = 8;
	this->_serialPort->StopBits = StopBits::One;

	this->fp = fopen( "data.txt", "w");

}

int SerialCommunication::Open()
{
	int i;
#if useFakeData == 0
	this->_serialPort->Open();
	int result = this->_serialPort->IsOpen;

	//this->_serialPort->Write("N");
	
	unsigned int channel = 0;
	unsigned char startCmd[4] = {0x4, 1<<channel, 0x0, 0x0};
	String ^cmdString = gcnew String(reinterpret_cast<char const*>(startCmd));
	unsigned char startResponse[4];
	
	this->_serialPort->Write(cmdString);

	for(i=0;i<4;i++)
		startResponse[i] = this->_serialPort->ReadByte();
	
	this->_serialPort->DataReceived += gcnew SerialDataReceivedEventHandler(this, &SerialCommunication::DataReceivedHandler);
	
	return result;
#else
	//testing without microcontroller - just add random points forever
	System::Threading::ThreadStart^ starter = gcnew System::Threading::ThreadStart(this, &SerialCommunication::Threadstarter);
	System::Threading::Thread^ thread = gcnew System::Threading::Thread(starter);


	thread->Start();

	return true;
#endif
}

void SerialCommunication::Close()
{
	this->_serialPort->Close();
}

void SerialCommunication::DataReceivedHandler(Object^ sender, SerialDataReceivedEventArgs^ e)
{
	while(this->_serialPort->BytesToRead > 2)
	{
		unsigned char dataArray[3];
		//dataUnion du;

		//this->_serialPort->Read(dataArray, 0, 3);
		
		dataArray[0] = this->_serialPort->ReadByte();
		dataArray[1] = this->_serialPort->ReadByte();
		dataArray[2] = this->_serialPort->ReadByte();

		/*
		unToSigned us;
		us.us = dataArray[1];
		int n = ((int) us.si) + 128;
		*/
		/*
		du.raw[0] = this->_serialPort->ReadByte();
		du.raw[1] = this->_serialPort->ReadByte();
		du.raw[2] = this->_serialPort->ReadByte();
		du.raw[3] = 0;
		

		//int n = (du.raw[0] << 24 + du.raw[1] << 16 + du.raw[0] << 8)/256;
		int n = du.raw[0];
		n = n<<24;
		n += du.raw[1] << 16;
		n += du.raw[2] << 8;
		n /= 256;
		*/
		int n = dataArray[0];
		n = n<<24;
		n += dataArray[1] << 16;
		n += dataArray[2] << 8;
		n /= 256;

		//int n = (int) du.twosComplement;// / (65536);
		//int n = this->_serialPort->ReadChar();
		//fprintf(this->fp, "%x %x %x - %x = %d\n", du.raw[0], du.raw[1], du.raw[2], n, n);
		fprintf(this->fp, "%x %x %x - %x = %d\n", dataArray[0], dataArray[1], dataArray[2], n, n);

		//n = n / 65536 + 128;
		//n = n / 2000;// + 128;
		this->_callbackFunction->Invoke((double)n);
	}

}
