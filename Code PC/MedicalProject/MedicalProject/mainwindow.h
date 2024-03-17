#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include "SerialCommunicationTrade.h"
#include "serialcommunication.h"
#include "qtimer.h"
#include "imageprocessingtrade.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

signals:
	void messageDispatch(QString, QString);

private:
	Ui::MainWindowClass ui;
	
	QTimer * mainWindowTimer;

	SerialCommunication * objSerialCommunication;

	QString windowWorkingIndicator = "";
	float masTrans_MW;
	float masTwist_MW;
	float slaTrans_MW;
	float slaTwist_MW;

private slots:
	void OnMWFileExit();
	void OnMWAboutDevelopers();
	void OnMWClose();
	void OnMWConnect();
	void messageDisplay(QString, QString);
	void OnTimeout();
};

#endif // MAINWINDOW_H