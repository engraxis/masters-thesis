#ifndef GLOBALVARIABLES_H
#define GLOBALVARIABLES_H

#include <QObject>
#include <mutex>

class SerialCommunicationTrade : public QObject
{
	Q_OBJECT

private:
	SerialCommunicationTrade(QObject *parent);
	~SerialCommunicationTrade();

public:
	static bool _isSerialPortInitialized; //Not necessarily needs to be mutex locked 
	static bool _isSerialPortSynchronized;//Not necessarily needs to be mutex locked

public:
	static void setMotorPositions(float, float, float, float);
	static float * getMotorPositions(void);
	
	static void setRxCount(int);
	static int getRxCount(void);
	
	static void setClutchStatus(bool);
	static bool getClutchStatus(void);

	static void setDataPacket(unsigned char *);
	static unsigned char * getDataPacket(void);

	static void setSyncStatus(bool);
	static bool getSyncStatus(void);

	static void setComRate(double);
	static double getComRate(void);

private:
	static std::mutex motorPositionsLock;
	static std::mutex rxCountLock;
	static std::mutex clutchLock;
	static std::mutex dataPacketLock;
	static std::mutex syncLock;
	static std::mutex rateMutex;

	static float masterPos[2];	//0: Translation, 1: Twisting
	static float slavePos[2];	//0: Translation, 1: Twisting
	static int dataRXCount;		//Number of bytes received in one data packet of 16 bytes
	static bool clutchStatus;
	static unsigned char dataPacket[16];
	static bool syncStatus;
	static double comRate;
};

#endif // GLOBALVARIABLES_H