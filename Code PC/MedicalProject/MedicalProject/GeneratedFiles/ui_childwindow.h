/********************************************************************************
** Form generated from reading UI file 'childwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHILDWINDOW_H
#define UI_CHILDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_childwindow
{
public:
    QLabel *label_rgbAngio;
    QLabel *label_rgbGWSegment;
    QLabel *label_rgbGWXray;
    QLabel *label_rgbGWCrpTangent;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;

    void setupUi(QWidget *childwindow)
    {
        if (childwindow->objectName().isEmpty())
            childwindow->setObjectName(QStringLiteral("childwindow"));
        childwindow->resize(891, 682);
        label_rgbAngio = new QLabel(childwindow);
        label_rgbAngio->setObjectName(QStringLiteral("label_rgbAngio"));
        label_rgbAngio->setGeometry(QRect(40, 30, 400, 300));
        label_rgbAngio->setStyleSheet(QStringLiteral("border: 1px solid black"));
        label_rgbGWSegment = new QLabel(childwindow);
        label_rgbGWSegment->setObjectName(QStringLiteral("label_rgbGWSegment"));
        label_rgbGWSegment->setGeometry(QRect(450, 30, 400, 300));
        label_rgbGWSegment->setStyleSheet(QStringLiteral("border: 1px solid black"));
        label_rgbGWXray = new QLabel(childwindow);
        label_rgbGWXray->setObjectName(QStringLiteral("label_rgbGWXray"));
        label_rgbGWXray->setGeometry(QRect(40, 370, 400, 300));
        label_rgbGWXray->setStyleSheet(QStringLiteral("border: 1px solid black"));
        label_rgbGWCrpTangent = new QLabel(childwindow);
        label_rgbGWCrpTangent->setObjectName(QStringLiteral("label_rgbGWCrpTangent"));
        label_rgbGWCrpTangent->setGeometry(QRect(450, 370, 161, 151));
        label_rgbGWCrpTangent->setStyleSheet(QStringLiteral("border: 1px solid black"));
        label_5 = new QLabel(childwindow);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(40, 0, 251, 31));
        QFont font;
        font.setPointSize(12);
        font.setBold(true);
        font.setWeight(75);
        label_5->setFont(font);
        label_6 = new QLabel(childwindow);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(40, 340, 251, 31));
        label_6->setFont(font);
        label_7 = new QLabel(childwindow);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(450, 0, 251, 31));
        label_7->setFont(font);
        label_8 = new QLabel(childwindow);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(450, 340, 271, 31));
        label_8->setFont(font);

        retranslateUi(childwindow);

        QMetaObject::connectSlotsByName(childwindow);
    } // setupUi

    void retranslateUi(QWidget *childwindow)
    {
        childwindow->setWindowTitle(QApplication::translate("childwindow", "childwindow", 0));
        label_rgbAngio->setText(QApplication::translate("childwindow", "TextLabel", 0));
        label_rgbGWSegment->setText(QApplication::translate("childwindow", "TextLabel", 0));
        label_rgbGWXray->setText(QApplication::translate("childwindow", "TextLabel", 0));
        label_rgbGWCrpTangent->setText(QApplication::translate("childwindow", "TextLabel", 0));
        label_5->setText(QApplication::translate("childwindow", "Angiogram Image - Live CAM", 0));
        label_6->setText(QApplication::translate("childwindow", "X-ray Image", 0));
        label_7->setText(QApplication::translate("childwindow", "Guidewire Segmentation", 0));
        label_8->setText(QApplication::translate("childwindow", "Guidewire Cropped and Tangent", 0));
    } // retranslateUi

};

namespace Ui {
    class childwindow: public Ui_childwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHILDWINDOW_H
