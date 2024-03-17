#include "mainwindow.h"

/* All initializations, SIGNAL-SLOT connections are done here
*/
MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	objSerialCommunication = new SerialCommunication(this);

	ui.setupUi(this);
	this->setFixedSize(this->size());
	this->setWindowTitle("Medical Project - Main Window");
	connect(ui.pushButton_Close, SIGNAL(clicked()), this, SLOT(OnMWClose()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(OnMWFileExit()));	
	connect(ui.actionAbout_Developers, SIGNAL(triggered()), this, SLOT(OnMWAboutDevelopers()));
	connect(ui.pushButton_Connect, SIGNAL(clicked()), this, SLOT(OnMWConnect()));
	connect(objSerialCommunication, SIGNAL(messageDispatch(QString, QString)), this, SLOT(messageDisplay(QString, QString)));
	connect(this, SIGNAL(messageDispatch(QString, QString)), this, SLOT(messageDisplay(QString, QString)));

	mainWindowTimer = new QTimer(this);
	connect(mainWindowTimer, SIGNAL(timeout()), this, SLOT(OnTimeout()));
	mainWindowTimer->start(33);
}

/* This function deletes the variables/objects
*/
MainWindow::~MainWindow()
{
	mainWindowTimer->stop();
}

/* This function is for Help->AboutDevelopers
** This function shows the information box of our lab
*/
void MainWindow::OnMWAboutDevelopers()
{
	QString title = "GIST HRLab";
	QString body;
	body  = "Human Robotics Laboratory \n\n";
	body += "\t Haptic Technology Research Center \n"; 
	body += "\t School of Mechanical Engineering \n";
	body += "\t Gwangju Institute of Science and Technology \n";
	body += "\t https://www.haptics.gist.ac.kr/ \n";
	body += "\t https://ewww.gist.ac.kr/ \n";
	QMessageBox::information(this, title, body);
}

/* This function is for pushButton_Close
** This function deletes the variables/objects
*/
void MainWindow::OnMWClose()
{
	delete objSerialCommunication;
	QApplication::exit();
}

/* This function is for File->Exit
** This function deletes the variables/objects
*/
void MainWindow::OnMWFileExit()
{
	delete objSerialCommunication;
	QApplication::exit();
}

/* This function is for pushButton_Connect
** This function calls SerialCommunication function to initializd port and creates a thread there
*/
void MainWindow::OnMWConnect()
{
	if (SerialCommunicationTrade::_isSerialPortInitialized == false)
	{
		QString comPort;
		QString baudRate;
		int comPortIndex;
		int baudRateIndex;
		comPortIndex = ui.comboBox_COMPort->currentIndex();
		baudRateIndex = ui.comboBox_BaudRate->currentIndex();
		if (comPortIndex == 0)
			comPort = "COM1";
		else
			comPort = ui.comboBox_COMPort->currentText();
		if (baudRateIndex == 0)
			baudRate = "460800";
		else
			baudRate = ui.comboBox_BaudRate->currentText();

		objSerialCommunication->createSerialThreads(comPort, baudRate);
	}
	else
	{
		emit messageDispatch("Warning", "Port is alreay OPEN. \nCheck:\t\tMainWindow::OnMWConnect()");
	}
}

/* This function is linked to the SIGNAL message (check construction for details)
** This function displays the information message box
*/
void MainWindow::messageDisplay(QString title, QString body)
{
	QMessageBox::information(this, title, body);
}

/* This function is linked to the SIGNAL timeout (check construction for details)
** This function updates the MainWindow lables and other ui widgets 
*/
void MainWindow::OnTimeout()
{
	//We can optimize this section by rendering the unchanged text in the contructor!
	windowWorkingIndicator = windowWorkingIndicator + "|";
	if (windowWorkingIndicator.length() >= 30)
		windowWorkingIndicator = "";
	ui.label_WindowAliveIndicator->setText(windowWorkingIndicator);

	//Block: Robot Positions
	float * motorPos = SerialCommunicationTrade::getMotorPositions();
	masTrans_MW = motorPos[0];
	masTwist_MW = motorPos[1];
	slaTrans_MW = motorPos[2];
	slaTwist_MW = motorPos[3];
	delete motorPos;
	QString convAndDisplay; 
	ui.label_1a->setText("Master Translation");			convAndDisplay = QString::number(masTrans_MW);							ui.label_1b->setText(convAndDisplay);
	ui.label_2a->setText("Master Twist");				convAndDisplay = QString::number(masTwist_MW);							ui.label_2b->setText(convAndDisplay);
	ui.label_3a->setText("Slave Translation");			convAndDisplay = QString::number(slaTrans_MW);							ui.label_3b->setText(convAndDisplay);
	ui.label_4a->setText("Slave Twist");				convAndDisplay = QString::number(slaTwist_MW);							ui.label_4b->setText(convAndDisplay);
	if (SerialCommunicationTrade::getClutchStatus())	convAndDisplay = "ON";	else convAndDisplay = "OFF";
	ui.label_4a1->setText("Clutch Switch");																						ui.label_4b1->setText(convAndDisplay);
	
	//Block: Guidewire Attributes
	double * tmpGWPos = ImageProcessingTrade::getGWPos();
	double xPos = tmpGWPos[0];	double yPos = tmpGWPos[1];
	delete tmpGWPos;
	ui.label_5a->setText("Tip Position");		convAndDisplay = QString::number(xPos);											ui.label_5b->setText("x: " + convAndDisplay);
												convAndDisplay = QString::number(yPos);											ui.label_5c->setText("y: " + convAndDisplay);
	ui.label_6a->setText("Bending Angle");		convAndDisplay = QString::number(ImageProcessingTrade::getTipBendAng());		ui.label_6b->setText(convAndDisplay);
	
	//Block: Force/Torque Field Attributes
	ui.label_7a->setText("Angle");				convAndDisplay = QString::number(ImageProcessingTrade::getFTFAngle());			ui.label_7b->setText(convAndDisplay);
	ui.label_7a1->setText("FTF Inside Index");	convAndDisplay = QString::number(HapticRenderingTrade::getftfInsideIndex());	ui.label_7b1->setText(convAndDisplay);
	ui.label_7a2->setText("FTF Type");			QString ftfType(ImageProcessingTrade::getFTFType());							ui.label_7b2->setText(ftfType);
	ui.label_7a3->setText("FTF Index");			convAndDisplay = QString::number(ImageProcessingTrade::getFTFMaxIndex());		ui.label_7b3->setText(convAndDisplay);

	//Block: Angle Difference
	ui.label_8a->setText("Angle Difference");	convAndDisplay = QString::number(ImageProcessingTrade::getIPAngVal());			ui.label_8b->setText(convAndDisplay);
	ui.label_9a->setText("Inner Product");		convAndDisplay = QString::number(ImageProcessingTrade::getIPResVal());			ui.label_9b->setText(convAndDisplay);
	
	//Block: Haptic Feedback
	double * getForTor = HapticRenderingTrade::getForTor();
	ui.label_10a->setText("Force");				convAndDisplay = QString::number(getForTor[0]);									ui.label_10b->setText(convAndDisplay);
	ui.label_11a->setText("Torque");			convAndDisplay = QString::number(getForTor[1]);									ui.label_11b->setText(convAndDisplay);
	delete getForTor;

	//Block: Status Line 1 (Times Elapsed)
	convAndDisplay = QString::number(ImageProcessingTrade::getImgProcRate());				ui.label_ImgProcRate->setText("Image Processing Rate = " + convAndDisplay + " fps");
	convAndDisplay = QString::number(HapticRenderingTrade::getHapRendRate());				ui.label_HapRendRate->setText("Haptic Rendering Rate = " + convAndDisplay + " Hz");
	convAndDisplay = QString::number(SerialCommunicationTrade::getComRate());				ui.label_SerComRate->setText("Serial Communication Rate = " + convAndDisplay + " Hz");

	//Block: Status Line 2
	QString strDataRXCount = QString::number(SerialCommunicationTrade::getRxCount());
	ui.label_StatusRxCount->setText("Rx Count: " + strDataRXCount);

	static bool bRunOnce = true;
	if (bRunOnce)
	{
		if (SerialCommunicationTrade::_isSerialPortInitialized)
		{
			ui.pushButton_Connect->setEnabled(false);
			ui.label_StatusSerialConnect->setText("Serial Port Connected.");
			if (SerialCommunicationTrade::getSyncStatus())
			{
				ui.label_StatusSerialConnect->setText("Serial Port Connected and Synchronized.");
				bRunOnce = false;
			}
		}
	}
}