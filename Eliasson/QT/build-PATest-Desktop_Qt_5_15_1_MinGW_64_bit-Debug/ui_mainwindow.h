/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QLabel *label;
    QComboBox *comboBoxSerialPorts;
    QPushButton *pushButtonConnectSerial;
    QCustomPlot *widgetPlot;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(971, 755);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(centralwidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        label = new QLabel(splitter);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(130, 0));
        label->setMaximumSize(QSize(130, 50));
        QFont font;
        font.setPointSize(20);
        label->setFont(font);
        splitter->addWidget(label);
        comboBoxSerialPorts = new QComboBox(splitter);
        comboBoxSerialPorts->setObjectName(QString::fromUtf8("comboBoxSerialPorts"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(comboBoxSerialPorts->sizePolicy().hasHeightForWidth());
        comboBoxSerialPorts->setSizePolicy(sizePolicy1);
        comboBoxSerialPorts->setMinimumSize(QSize(0, 50));
        comboBoxSerialPorts->setMaximumSize(QSize(16777215, 50));
        QFont font1;
        font1.setPointSize(10);
        comboBoxSerialPorts->setFont(font1);
        splitter->addWidget(comboBoxSerialPorts);
        pushButtonConnectSerial = new QPushButton(splitter);
        pushButtonConnectSerial->setObjectName(QString::fromUtf8("pushButtonConnectSerial"));
        sizePolicy1.setHeightForWidth(pushButtonConnectSerial->sizePolicy().hasHeightForWidth());
        pushButtonConnectSerial->setSizePolicy(sizePolicy1);
        pushButtonConnectSerial->setMaximumSize(QSize(150, 50));
        pushButtonConnectSerial->setFont(font);
        pushButtonConnectSerial->setCheckable(true);
        pushButtonConnectSerial->setChecked(true);
        splitter->addWidget(pushButtonConnectSerial);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        widgetPlot = new QCustomPlot(centralwidget);
        widgetPlot->setObjectName(QString::fromUtf8("widgetPlot"));
        widgetPlot->setMinimumSize(QSize(0, 500));

        gridLayout->addWidget(widgetPlot, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 971, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Serial Port", nullptr));
        pushButtonConnectSerial->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
