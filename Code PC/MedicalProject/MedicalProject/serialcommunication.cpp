#include "serialcommunication.h"
#include "mainwindow.h"

SerialCommunication::SerialCommunication(QObject *parent)
	: QObject(parent)
{
	objHapticRendering = new HapticRendering();
	ucharRXData[16] = { 0, };
	extractedRXData[4] = { 0, };
	readData = "";
	writeData = "";
	masTrans_SC = 0.0;
	masTwist_SC = 0.0;
	slaTrans_SC = 0.0;
	slaTwist_SC = 0.0;
	isClutchActv = false;
}

SerialCommunication::~SerialCommunication()
{
	terminateSerialCommunicationActivities();
}

/* This function is called by the destructor (~SerialCommunication)
** This function deletes the variables/objects and closes the thread
*/
void SerialCommunication::terminateSerialCommunicationActivities()
{
	objHapticRendering->endHapticThread();
	if (threadRun)
	{
		threadRun = false;
		_sleep(100);				//Wait until thread loop is finished
		readSerialThread->join();		//Wait until thread is ended
	}
	delete objHapticRendering;
}

/* This function is called by the Connect button on MainWindow (pushbutton_Connect -> OnMWConnect)
** This function creates the thread for serial communication
*/
void SerialCommunication::createSerialThreads(QString COMPort, QString BaudRate)
{
	threadRun = true;
	readSerialThread = new std::thread(&SerialCommunication::readThread, this, COMPort, BaudRate);	//Creating standard thread

	objHapticRendering->createHapticThread();
}

/* This function is called by the createSerialThread
** This function periodically sends and receives the data from COM port
*/
void SerialCommunication::readThread(QString COMPort, QString BaudRate)
{
#pragma region SettingUpSerialPort
	QSerialPort mySerialPort;
	mySerialPort.setPortName(COMPort);
	if (BaudRate == "460800")
		mySerialPort.setBaudRate(460800);
	else
		emit messageDispatch("Error", "Error in setting up baudrate.\nCheck:\t\tSerialCommunication::serialThread(QString COMPort, QString BaudRate)");
	mySerialPort.setFlowControl(QSerialPort::NoFlowControl);
	mySerialPort.setDataBits(QSerialPort::Data8);
	mySerialPort.setStopBits(QSerialPort::OneStop);
	mySerialPort.setParity(QSerialPort::NoParity);
	if (true == mySerialPort.open(QIODevice::ReadWrite))
	{
		//Serial port successfully opened
		SerialCommunicationTrade::_isSerialPortInitialized = true;
	}
	else
	{
		//Error in opening port
		SerialCommunicationTrade::_isSerialPortInitialized = false;
		emit messageDispatch("Error", "Error in opening COM port.\nCheck:\t\tSerialCommunication::serialThread(QString COMPort, QString BaudRate)");
	}

	readData.clear();
	readData = "";
	mySerialPort.clear(QSerialPort::AllDirections); //Clearing port may result in loss of data
#pragma endregion
	SYSTEMTIME time;
	unsigned long int timeStart = 0;
	unsigned long int timeEnd = 0;
	unsigned long int timeElapsed = 0;
	while (threadRun)
	{
		GetSystemTime(&time);
		timeStart = (time.wSecond * 1000) + time.wMilliseconds;
		//This loop works until Close or Exit is pressed from MainWindow
		//This loop continuously sends data to serial port and watches if there is any incoming data
		//Better strategy could be made by connecting serialPort::dataReady signal with dataRead customized function
		_sleep(1);
		unsigned char * dataToWrite = SerialCommunicationTrade::getDataPacket();
		QByteArray writeBuf = QByteArray((char*)dataToWrite, 16);
		mySerialPort.write(writeBuf);
		mySerialPort.waitForBytesWritten(WRITE_TIMEOUT);
		delete dataToWrite;

		if (true == mySerialPort.waitForReadyRead(READ_TIMEOUT))
		{
			readData.clear();
			readData = "";
			readData = mySerialPort.readAll();
			SerialCommunicationTrade::setRxCount(readData.length());
			mySerialPort.clear(QSerialPort::AllDirections);

			int index = readData.length() - 17;
			// Find the packet index
			while (index >= 0) 
			{
				// Test for packet format
				if (readData.at(index)     == 'A' &&
					readData.at(index + 1) == 'L' &&
					readData.at(index + 2) == 'L' &&
					readData.at(index + 3) == 'R') 
				{
					
					memcpy(ucharRXData, &(readData.toStdString().c_str()[index]), 16);
					dataHandling();		//Upon every data received, perform data handling
					break;
				}
				else if (readData.at(index)      == 101 &&
					readData.at(index + 4)  == 102 &&
					readData.at(index + 8)  == 103 &&
					readData.at(index + 12) == 104)
				{

					memcpy(ucharRXData, &(readData.toStdString().c_str()[index]), 16);
					dataHandling();		//Upon every data received, perform data handling
					break;
				}
				else if (readData.at(index) == 97 &&
					readData.at(index + 4)  == 98 &&
					readData.at(index + 8)  == 99 &&
					readData.at(index + 12) == 100)
				{

					memcpy(ucharRXData, &(readData.toStdString().c_str()[index]), 16);
					dataHandling();		//Upon every data received, perform data handling
					break;
				}
				else
				{
					index--;
				}
			}

		}
		GetSystemTime(&time);
		timeEnd = (time.wSecond * 1000) + time.wMilliseconds;
		timeElapsed = timeEnd - timeStart;

		//Make following algorithm to be moving average for better visualization
		static int cnt = 0;
		const int NumOfSamples = 50;
		static unsigned long int timeBuf[NumOfSamples] = { 0, };
		timeBuf[cnt] = timeElapsed;
		cnt++;
		if (cnt == NumOfSamples)
		{
			for (int loop = 0; loop < NumOfSamples; loop++)
				timeElapsed += timeBuf[loop];
			timeElapsed /= NumOfSamples;
			cnt = 0;
			SerialCommunicationTrade::setComRate(1000.0 / ((double)timeElapsed));
		}
		
	}
	mySerialPort.close();
}

/* This function is called by the serialThread
** This function performs data parsing and stores data in shared variables
*/
void SerialCommunication::dataHandling(void)
{
	//Extract data based upon header
	if (ucharRXData[0] == 'A' && ucharRXData[1] == 'L' && ucharRXData[2] == 'L' && ucharRXData[3] == 'R' && ucharRXData[4] == 'E' && ucharRXData[5] == 'A' && ucharRXData[6] == 'D' && ucharRXData[7] == 'Y')
	{
		SerialCommunicationTrade::_isSerialPortSynchronized = true;
		SerialCommunicationTrade::setSyncStatus(true);
		if (!HapticRendering::isHapticThreadRunning) 
		{
		//	objHapticRendering->createHapticThread();
		}
	}
	else if (ucharRXData[0] == 101 && ucharRXData[4] == 102 && ucharRXData[8] == 103 && ucharRXData[12] == 104)
	{
		isClutchActv = ucharRXData[1];
		SerialCommunicationTrade::setClutchStatus(isClutchActv);
	}
	else if (ucharRXData[0] == 97 && ucharRXData[4] == 98 && ucharRXData[8] == 99 && ucharRXData[12] == 100)
	{
		if (ucharRXData[0] == 97)
		{
			if (ucharRXData[1] & 0x80)
			{
				extractedRXData[0]  = 0xFF000000;
				extractedRXData[0] |= (ucharRXData[1] << 16);
				extractedRXData[0] |= (ucharRXData[2] << 8);
				extractedRXData[0] |= ucharRXData[3];
			}
			else
			{
				extractedRXData[0] = (ucharRXData[1] << 16);
				extractedRXData[0] |= (ucharRXData[2] << 8);
				extractedRXData[0] |= ucharRXData[3];
			}
		}
		if (ucharRXData[4] == 98)
		{
			if (ucharRXData[5] & 0x80)
			{
				extractedRXData[1]  = 0xFF000000;
				extractedRXData[1] |= (ucharRXData[5] << 16);
				extractedRXData[1] |= (ucharRXData[6] << 8);
				extractedRXData[1] |=  ucharRXData[7];
			}
			else
			{
				extractedRXData[1]  = (ucharRXData[5] << 16);
				extractedRXData[1] |= (ucharRXData[6] << 8);
				extractedRXData[1] |=  ucharRXData[7];
			}
		}
		if (ucharRXData[8] == 99)
		{
			if (ucharRXData[9] & 0x80)
			{
				extractedRXData[2]  = 0xFF000000;
				extractedRXData[2] |= (ucharRXData[9] << 16);
				extractedRXData[2] |= (ucharRXData[10] << 8);
				extractedRXData[2] |=  ucharRXData[11];
			}
			else
			{
				extractedRXData[2]  = (ucharRXData[9] << 16);
				extractedRXData[2] |= (ucharRXData[10] << 8);
				extractedRXData[2] |=  ucharRXData[11];
			}
		}
		if (ucharRXData[12] == 100)
		{
			if (ucharRXData[13] & 0x80)
			{
				extractedRXData[3]  = 0xFF000000;
				extractedRXData[3] |= (ucharRXData[13] << 16);
				extractedRXData[3] |= (ucharRXData[14] << 8);
				extractedRXData[3] |=  ucharRXData[15];
			}
			else
			{
				extractedRXData[3]  = (ucharRXData[13] << 16);
				extractedRXData[3] |= (ucharRXData[14] << 8);
				extractedRXData[3] |=  ucharRXData[15];
			}
		}
		masTrans_SC = (((float)(long)extractedRXData[0]) * 0.001) * 10;
		masTwist_SC = (((float)(long)extractedRXData[1]) * 0.001) * 10;
		slaTrans_SC = (((float)(long)extractedRXData[2]) * 0.001) * 10;
		slaTwist_SC = (((float)(long)extractedRXData[3]) * 0.001) * 10;
		SerialCommunicationTrade::setMotorPositions(masTrans_SC, masTwist_SC, slaTrans_SC, slaTwist_SC);
	}// </end of 16 bytes data packet processing>
}