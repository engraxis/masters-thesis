#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <thread>
#include <QtSerialPort\qserialport.h>
#include <qmessagebox.h>
#include "SerialCommunicationTrade.h"
#include "HapticRendering.h"
#include <Windows.h>

class SerialCommunication : public QObject
{
	Q_OBJECT

public:
	SerialCommunication(QObject *parent);
	~SerialCommunication();

public:
	void createSerialThreads(QString, QString);

signals:
	void messageDispatch(QString, QString);

private:
	void readThread(QString, QString);
	void dataHandling(void);
	void terminateSerialCommunicationActivities();
	HapticRendering * objHapticRendering;
	

private:
	#define READ_TIMEOUT 1
	#define	WRITE_TIMEOUT 5
	#define NUM_RX_DATA 16
	std::thread * readSerialThread;
	std::thread * writeSerialThread;
	QByteArray readData;
	bool threadRun = false;
	unsigned char ucharRXData[16];
	unsigned long extractedRXData[4];
	float masTrans_SC;
	float masTwist_SC;
	float slaTrans_SC;
	float slaTwist_SC;
	bool isClutchActv;
	QByteArray writeData;
};

#endif // SERIALCOMMUNICATION_H
