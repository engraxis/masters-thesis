#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "childwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow objMainWindow;
	childwindow objChildWindow;
	objMainWindow.show();
	objChildWindow.show();
	return a.exec();
}