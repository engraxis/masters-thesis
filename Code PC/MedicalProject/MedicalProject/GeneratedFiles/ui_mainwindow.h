/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionExit;
    QAction *actionAbout_Developers;
    QWidget *centralWidget;
    QPushButton *pushButton_Close;
    QGroupBox *groupBox;
    QComboBox *comboBox_COMPort;
    QComboBox *comboBox_BaudRate;
    QPushButton *pushButton_Connect;
    QLabel *label_StatusSerialConnect;
    QFrame *line;
    QLabel *label_WindowAliveIndicator;
    QLabel *label_StatusRxCount;
    QGroupBox *groupBox_2;
    QLabel *label_4a;
    QLabel *label_3a;
    QLabel *label_1a;
    QLabel *label_4b;
    QLabel *label_2a;
    QLabel *label_1b;
    QLabel *label_2b;
    QLabel *label_3b;
    QLabel *label_4a1;
    QLabel *label_4b1;
    QGroupBox *groupBox_3;
    QLabel *label_5a;
    QLabel *label_6b;
    QLabel *label_6a;
    QLabel *label_5b;
    QLabel *label_5c;
    QGroupBox *groupBox_4;
    QLabel *label_7a;
    QLabel *label_7b;
    QLabel *label_7a1;
    QLabel *label_7b1;
    QLabel *label_7b3;
    QLabel *label_7b2;
    QLabel *label_7a2;
    QLabel *label_7a3;
    QGroupBox *groupBox_5;
    QLabel *label_8a;
    QLabel *label_8b;
    QLabel *label_9b;
    QLabel *label_9a;
    QGroupBox *groupBox_6;
    QLabel *label_10a;
    QLabel *label_10b;
    QLabel *label_11a;
    QLabel *label_11b;
    QLabel *label_ImgProcRate;
    QLabel *label_HapRendRate;
    QLabel *label_SerComRate;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(669, 576);
        MainWindowClass->setStyleSheet(QStringLiteral(""));
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAbout_Developers = new QAction(MainWindowClass);
        actionAbout_Developers->setObjectName(QStringLiteral("actionAbout_Developers"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton_Close = new QPushButton(centralWidget);
        pushButton_Close->setObjectName(QStringLiteral("pushButton_Close"));
        pushButton_Close->setGeometry(QRect(580, 460, 75, 23));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 221, 81));
        QFont font;
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        groupBox->setFont(font);
        groupBox->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        comboBox_COMPort = new QComboBox(groupBox);
        comboBox_COMPort->setObjectName(QStringLiteral("comboBox_COMPort"));
        comboBox_COMPort->setGeometry(QRect(10, 20, 121, 20));
        comboBox_BaudRate = new QComboBox(groupBox);
        comboBox_BaudRate->setObjectName(QStringLiteral("comboBox_BaudRate"));
        comboBox_BaudRate->setGeometry(QRect(10, 50, 122, 20));
        pushButton_Connect = new QPushButton(groupBox);
        pushButton_Connect->setObjectName(QStringLiteral("pushButton_Connect"));
        pushButton_Connect->setGeometry(QRect(140, 20, 71, 51));
        QFont font1;
        font1.setBold(false);
        font1.setWeight(50);
        pushButton_Connect->setFont(font1);
        pushButton_Connect->setStyleSheet(QStringLiteral(""));
        label_StatusSerialConnect = new QLabel(centralWidget);
        label_StatusSerialConnect->setObjectName(QStringLiteral("label_StatusSerialConnect"));
        label_StatusSerialConnect->setGeometry(QRect(20, 510, 211, 16));
        QFont font2;
        font2.setItalic(false);
        label_StatusSerialConnect->setFont(font2);
        label_StatusSerialConnect->setStyleSheet(QStringLiteral("color: rgb(85, 0, 0)"));
        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(0, 480, 661, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_WindowAliveIndicator = new QLabel(centralWidget);
        label_WindowAliveIndicator->setObjectName(QStringLiteral("label_WindowAliveIndicator"));
        label_WindowAliveIndicator->setGeometry(QRect(580, 505, 71, 20));
        label_WindowAliveIndicator->setStyleSheet(QStringLiteral("color:rgb(85, 0, 0)"));
        label_StatusRxCount = new QLabel(centralWidget);
        label_StatusRxCount->setObjectName(QStringLiteral("label_StatusRxCount"));
        label_StatusRxCount->setGeometry(QRect(240, 510, 81, 16));
        label_StatusRxCount->setStyleSheet(QStringLiteral("color:rgb(85, 0, 0)"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 100, 221, 121));
        QFont font3;
        font3.setBold(true);
        font3.setWeight(75);
        groupBox_2->setFont(font3);
        groupBox_2->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        label_4a = new QLabel(groupBox_2);
        label_4a->setObjectName(QStringLiteral("label_4a"));
        label_4a->setGeometry(QRect(10, 80, 100, 13));
        label_4a->setFont(font1);
        label_3a = new QLabel(groupBox_2);
        label_3a->setObjectName(QStringLiteral("label_3a"));
        label_3a->setGeometry(QRect(10, 60, 100, 13));
        label_3a->setFont(font1);
        label_1a = new QLabel(groupBox_2);
        label_1a->setObjectName(QStringLiteral("label_1a"));
        label_1a->setGeometry(QRect(10, 20, 100, 13));
        label_1a->setFont(font1);
        label_4b = new QLabel(groupBox_2);
        label_4b->setObjectName(QStringLiteral("label_4b"));
        label_4b->setGeometry(QRect(160, 80, 47, 13));
        label_4b->setFont(font1);
        label_2a = new QLabel(groupBox_2);
        label_2a->setObjectName(QStringLiteral("label_2a"));
        label_2a->setGeometry(QRect(10, 40, 100, 13));
        label_2a->setFont(font1);
        label_1b = new QLabel(groupBox_2);
        label_1b->setObjectName(QStringLiteral("label_1b"));
        label_1b->setGeometry(QRect(160, 20, 47, 13));
        label_1b->setFont(font1);
        label_2b = new QLabel(groupBox_2);
        label_2b->setObjectName(QStringLiteral("label_2b"));
        label_2b->setGeometry(QRect(160, 40, 47, 13));
        label_2b->setFont(font1);
        label_3b = new QLabel(groupBox_2);
        label_3b->setObjectName(QStringLiteral("label_3b"));
        label_3b->setGeometry(QRect(160, 60, 47, 13));
        label_3b->setFont(font1);
        label_4a1 = new QLabel(groupBox_2);
        label_4a1->setObjectName(QStringLiteral("label_4a1"));
        label_4a1->setGeometry(QRect(10, 100, 81, 16));
        label_4b1 = new QLabel(groupBox_2);
        label_4b1->setObjectName(QStringLiteral("label_4b1"));
        label_4b1->setGeometry(QRect(160, 100, 47, 13));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 230, 221, 71));
        groupBox_3->setFont(font3);
        groupBox_3->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        label_5a = new QLabel(groupBox_3);
        label_5a->setObjectName(QStringLiteral("label_5a"));
        label_5a->setGeometry(QRect(10, 20, 100, 13));
        label_5a->setFont(font1);
        label_6b = new QLabel(groupBox_3);
        label_6b->setObjectName(QStringLiteral("label_6b"));
        label_6b->setGeometry(QRect(160, 50, 47, 13));
        label_6b->setFont(font1);
        label_6a = new QLabel(groupBox_3);
        label_6a->setObjectName(QStringLiteral("label_6a"));
        label_6a->setGeometry(QRect(10, 50, 100, 13));
        label_6a->setFont(font1);
        label_5b = new QLabel(groupBox_3);
        label_5b->setObjectName(QStringLiteral("label_5b"));
        label_5b->setGeometry(QRect(160, 20, 41, 16));
        label_5b->setFont(font1);
        label_5c = new QLabel(groupBox_3);
        label_5c->setObjectName(QStringLiteral("label_5c"));
        label_5c->setGeometry(QRect(160, 30, 41, 16));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 310, 221, 101));
        groupBox_4->setFont(font3);
        groupBox_4->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        label_7a = new QLabel(groupBox_4);
        label_7a->setObjectName(QStringLiteral("label_7a"));
        label_7a->setGeometry(QRect(10, 20, 100, 13));
        label_7a->setFont(font1);
        label_7b = new QLabel(groupBox_4);
        label_7b->setObjectName(QStringLiteral("label_7b"));
        label_7b->setGeometry(QRect(160, 20, 47, 13));
        label_7b->setFont(font1);
        label_7a1 = new QLabel(groupBox_4);
        label_7a1->setObjectName(QStringLiteral("label_7a1"));
        label_7a1->setGeometry(QRect(10, 40, 101, 16));
        label_7b1 = new QLabel(groupBox_4);
        label_7b1->setObjectName(QStringLiteral("label_7b1"));
        label_7b1->setGeometry(QRect(160, 40, 47, 13));
        label_7b3 = new QLabel(groupBox_4);
        label_7b3->setObjectName(QStringLiteral("label_7b3"));
        label_7b3->setGeometry(QRect(160, 80, 47, 13));
        label_7b2 = new QLabel(groupBox_4);
        label_7b2->setObjectName(QStringLiteral("label_7b2"));
        label_7b2->setGeometry(QRect(160, 60, 47, 13));
        label_7a2 = new QLabel(groupBox_4);
        label_7a2->setObjectName(QStringLiteral("label_7a2"));
        label_7a2->setGeometry(QRect(10, 60, 91, 16));
        label_7a3 = new QLabel(groupBox_4);
        label_7a3->setObjectName(QStringLiteral("label_7a3"));
        label_7a3->setGeometry(QRect(10, 80, 111, 16));
        groupBox_5 = new QGroupBox(centralWidget);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 420, 221, 61));
        groupBox_5->setFont(font3);
        groupBox_5->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        label_8a = new QLabel(groupBox_5);
        label_8a->setObjectName(QStringLiteral("label_8a"));
        label_8a->setGeometry(QRect(10, 20, 100, 13));
        label_8a->setFont(font1);
        label_8b = new QLabel(groupBox_5);
        label_8b->setObjectName(QStringLiteral("label_8b"));
        label_8b->setGeometry(QRect(160, 20, 47, 13));
        label_8b->setFont(font1);
        label_9b = new QLabel(groupBox_5);
        label_9b->setObjectName(QStringLiteral("label_9b"));
        label_9b->setGeometry(QRect(160, 40, 47, 13));
        label_9b->setFont(font1);
        label_9a = new QLabel(groupBox_5);
        label_9a->setObjectName(QStringLiteral("label_9a"));
        label_9a->setGeometry(QRect(10, 40, 100, 13));
        label_9a->setFont(font1);
        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(240, 10, 221, 61));
        groupBox_6->setFont(font3);
        groupBox_6->setStyleSheet(QLatin1String("QGroupBox { \n"
"     border: 1px solid gray; \n"
"     border-radius: 1px; \n"
" } "));
        label_10a = new QLabel(groupBox_6);
        label_10a->setObjectName(QStringLiteral("label_10a"));
        label_10a->setGeometry(QRect(10, 20, 111, 16));
        label_10b = new QLabel(groupBox_6);
        label_10b->setObjectName(QStringLiteral("label_10b"));
        label_10b->setGeometry(QRect(160, 20, 47, 13));
        label_11a = new QLabel(groupBox_6);
        label_11a->setObjectName(QStringLiteral("label_11a"));
        label_11a->setGeometry(QRect(10, 40, 121, 16));
        label_11b = new QLabel(groupBox_6);
        label_11b->setObjectName(QStringLiteral("label_11b"));
        label_11b->setGeometry(QRect(160, 40, 47, 13));
        label_ImgProcRate = new QLabel(centralWidget);
        label_ImgProcRate->setObjectName(QStringLiteral("label_ImgProcRate"));
        label_ImgProcRate->setGeometry(QRect(20, 490, 211, 16));
        label_ImgProcRate->setFont(font2);
        label_ImgProcRate->setStyleSheet(QStringLiteral("color: rgb(85, 0, 0)"));
        label_HapRendRate = new QLabel(centralWidget);
        label_HapRendRate->setObjectName(QStringLiteral("label_HapRendRate"));
        label_HapRendRate->setGeometry(QRect(240, 490, 211, 16));
        label_HapRendRate->setFont(font2);
        label_HapRendRate->setStyleSheet(QStringLiteral("color: rgb(85, 0, 0)"));
        label_SerComRate = new QLabel(centralWidget);
        label_SerComRate->setObjectName(QStringLiteral("label_SerComRate"));
        label_SerComRate->setGeometry(QRect(460, 490, 201, 16));
        label_SerComRate->setFont(font2);
        label_SerComRate->setStyleSheet(QStringLiteral("color: rgb(85, 0, 0)"));
        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 669, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindowClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        toolBar = new QToolBar(MainWindowClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBar);
        toolBar_2 = new QToolBar(MainWindowClass);
        toolBar_2->setObjectName(QStringLiteral("toolBar_2"));
        MainWindowClass->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout_Developers);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "MainWindow", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "Exit", 0));
        actionAbout_Developers->setText(QApplication::translate("MainWindowClass", "About Developers", 0));
        pushButton_Close->setText(QApplication::translate("MainWindowClass", "Close", 0));
        groupBox->setTitle(QApplication::translate("MainWindowClass", "Serial Port Settings", 0));
        comboBox_COMPort->clear();
        comboBox_COMPort->insertItems(0, QStringList()
         << QApplication::translate("MainWindowClass", "Select Port", 0)
         << QApplication::translate("MainWindowClass", "COM1", 0)
         << QApplication::translate("MainWindowClass", "COM2", 0)
         << QApplication::translate("MainWindowClass", "COM3", 0)
         << QApplication::translate("MainWindowClass", "COM4", 0)
         << QApplication::translate("MainWindowClass", "COM5", 0)
        );
        comboBox_BaudRate->clear();
        comboBox_BaudRate->insertItems(0, QStringList()
         << QApplication::translate("MainWindowClass", "Select Baud Rate", 0)
         << QApplication::translate("MainWindowClass", "460800", 0)
        );
#ifndef QT_NO_TOOLTIP
        pushButton_Connect->setToolTip(QApplication::translate("MainWindowClass", "<html><head/><body><p><span style=\" color:#550000;\">Click to connect to selected COM port with selected baudrate.</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        pushButton_Connect->setText(QApplication::translate("MainWindowClass", "Connect", 0));
        label_StatusSerialConnect->setText(QApplication::translate("MainWindowClass", "Serial Port Not Connected.", 0));
        label_WindowAliveIndicator->setText(QApplication::translate("MainWindowClass", "Window Alive", 0));
        label_StatusRxCount->setText(QApplication::translate("MainWindowClass", "Rx Count:", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindowClass", "Robot Positions", 0));
        label_4a->setText(QApplication::translate("MainWindowClass", "label_4a", 0));
        label_3a->setText(QApplication::translate("MainWindowClass", "label_3a", 0));
        label_1a->setText(QApplication::translate("MainWindowClass", "label_1a", 0));
        label_4b->setText(QApplication::translate("MainWindowClass", "label_4b", 0));
        label_2a->setText(QApplication::translate("MainWindowClass", "label_2a", 0));
        label_1b->setText(QApplication::translate("MainWindowClass", "label_1b", 0));
        label_2b->setText(QApplication::translate("MainWindowClass", "label_2b", 0));
        label_3b->setText(QApplication::translate("MainWindowClass", "label_3b", 0));
        label_4a1->setText(QApplication::translate("MainWindowClass", "label_4a1", 0));
        label_4b1->setText(QApplication::translate("MainWindowClass", "label_4b1", 0));
        groupBox_3->setTitle(QApplication::translate("MainWindowClass", "Guidewire Attributes", 0));
        label_5a->setText(QApplication::translate("MainWindowClass", "label_5a", 0));
        label_6b->setText(QApplication::translate("MainWindowClass", "label_6b", 0));
        label_6a->setText(QApplication::translate("MainWindowClass", "label_6a", 0));
        label_5b->setText(QApplication::translate("MainWindowClass", "label_5b", 0));
        label_5c->setText(QApplication::translate("MainWindowClass", "label_5c", 0));
        groupBox_4->setTitle(QApplication::translate("MainWindowClass", "Force/Torque Field Attributes", 0));
        label_7a->setText(QApplication::translate("MainWindowClass", "label_7a", 0));
        label_7b->setText(QApplication::translate("MainWindowClass", "label_7b", 0));
        label_7a1->setText(QApplication::translate("MainWindowClass", "label_7a1", 0));
        label_7b1->setText(QApplication::translate("MainWindowClass", "label_7b1", 0));
        label_7b3->setText(QApplication::translate("MainWindowClass", "label_7b3", 0));
        label_7b2->setText(QApplication::translate("MainWindowClass", "label_7b2", 0));
        label_7a2->setText(QApplication::translate("MainWindowClass", "label_7a2", 0));
        label_7a3->setText(QApplication::translate("MainWindowClass", "label_7a3", 0));
        groupBox_5->setTitle(QApplication::translate("MainWindowClass", "Angle Difference", 0));
        label_8a->setText(QApplication::translate("MainWindowClass", "label_8a", 0));
        label_8b->setText(QApplication::translate("MainWindowClass", "label_8b", 0));
        label_9b->setText(QApplication::translate("MainWindowClass", "label_9b", 0));
        label_9a->setText(QApplication::translate("MainWindowClass", "label_9a", 0));
        groupBox_6->setTitle(QApplication::translate("MainWindowClass", "Hatic Feedback", 0));
        label_10a->setText(QApplication::translate("MainWindowClass", "label_10a", 0));
        label_10b->setText(QApplication::translate("MainWindowClass", "label_10b", 0));
        label_11a->setText(QApplication::translate("MainWindowClass", "label_11a", 0));
        label_11b->setText(QApplication::translate("MainWindowClass", "label_11b", 0));
        label_ImgProcRate->setText(QApplication::translate("MainWindowClass", "Image Processing Rate = ", 0));
        label_HapRendRate->setText(QApplication::translate("MainWindowClass", "Haptic Rendering Rate = ", 0));
        label_SerComRate->setText(QApplication::translate("MainWindowClass", "Serial Communication Rate =", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("MainWindowClass", "Help", 0));
        toolBar->setWindowTitle(QApplication::translate("MainWindowClass", "toolBar", 0));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindowClass", "toolBar_2", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
