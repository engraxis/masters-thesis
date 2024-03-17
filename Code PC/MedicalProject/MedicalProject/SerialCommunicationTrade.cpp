#include "SerialCommunicationTrade.h"

std::mutex SerialCommunicationTrade::motorPositionsLock;
std::mutex SerialCommunicationTrade::rxCountLock;
std::mutex SerialCommunicationTrade::clutchLock;
std::mutex SerialCommunicationTrade::dataPacketLock;
std::mutex SerialCommunicationTrade::syncLock;
std::mutex SerialCommunicationTrade::rateMutex;

bool SerialCommunicationTrade::_isSerialPortInitialized = false;
bool SerialCommunicationTrade::_isSerialPortSynchronized = false;
float SerialCommunicationTrade::masterPos[2]; 
float SerialCommunicationTrade::slavePos[2];  
int SerialCommunicationTrade::dataRXCount;	
bool SerialCommunicationTrade::clutchStatus;
unsigned char SerialCommunicationTrade::dataPacket[16] = { 0, };
bool SerialCommunicationTrade::syncStatus = false;
double SerialCommunicationTrade::comRate;

SerialCommunicationTrade::SerialCommunicationTrade(QObject *parent)
	: QObject(parent)
{
	masterPos[0] = 0.0;
	masterPos[1] = 0.0;
	slavePos[0] = 0.0;
	slavePos[1] = 0.0;
	dataRXCount = 0;
	clutchStatus = false;
}

SerialCommunicationTrade::~SerialCommunicationTrade()
{

}

/* Setting the received data count vales, protected by mutexdf
*/
void SerialCommunicationTrade::setRxCount(int value)
{
	rxCountLock.lock();
	dataRXCount = value;
	rxCountLock.unlock();
}

/* Getting the received data count value, protected by mutex
*/
int SerialCommunicationTrade::getRxCount(void)
{
	
	rxCountLock.lock();
	int value = dataRXCount;
	rxCountLock.unlock();
	return value;
}

void SerialCommunicationTrade::setClutchStatus(bool val)
{
	clutchLock.lock();
	clutchStatus = val;
	clutchLock.unlock();
}

bool SerialCommunicationTrade::getClutchStatus(void)
{
	clutchLock.lock();
	bool ret = clutchStatus;
	clutchLock.unlock();
	return ret;
}

void SerialCommunicationTrade::setDataPacket(unsigned char * val)
{
	dataPacketLock.lock();
	for (int loop = 0; loop < 16; loop++)
		dataPacket[loop] = val[loop];
	dataPacketLock.unlock();
}

unsigned char * SerialCommunicationTrade::getDataPacket(void)
{
	unsigned char * cDataPacket;
	dataPacketLock.lock();
	cDataPacket = new unsigned char[16];
	for (int loop = 0; loop < 16; loop++)
		cDataPacket[loop] = dataPacket[loop];
	dataPacketLock.unlock();
	
	return cDataPacket;
}

void SerialCommunicationTrade::setSyncStatus(bool val)
{
	syncLock.lock();
	syncStatus = val;
	syncLock.unlock();
}

bool SerialCommunicationTrade::getSyncStatus(void)
{
	syncLock.lock();
	bool ret = syncStatus;
	syncLock.unlock();
	return ret;
}

void SerialCommunicationTrade::setComRate(double val)
{
	rateMutex.lock();
	comRate = val;
	rateMutex.unlock();
}

double SerialCommunicationTrade::getComRate(void)
{
	rateMutex.lock();
	double ret = comRate;
	rateMutex.unlock();
	return ret;
}

/* Setting the motor positions, protected by mutex
*/
void SerialCommunicationTrade::setMotorPositions(float masTrans, float masTwist, float slaTrans, float slaTwist)
{
	motorPositionsLock.lock();
	masterPos[0] = masTrans;
	masterPos[1] = masTwist;
	slavePos[0]  = slaTrans;
	slavePos[1]  = slaTwist;
	motorPositionsLock.unlock();
}

/* Getting the motor positions, protected by mutex
*/
float * SerialCommunicationTrade::getMotorPositions(void)
{
	motorPositionsLock.lock();
	float * ret = new float[4];
	ret[0] = masterPos[0];	//Master Translation
	ret[1] = masterPos[1];	//Master Twist
	ret[2] = slavePos[0];	//Slave Translation
	ret[3] = slavePos[1];	//Slave Twist
	motorPositionsLock.unlock();
	return ret;
}