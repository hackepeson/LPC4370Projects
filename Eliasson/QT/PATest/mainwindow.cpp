#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>



void ownSleep(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec,&loop,SLOT(quit()));
    loop.exec();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    updateSerialPorts(0);

    connect(ui->comboBoxSerialPorts, SIGNAL(activated(int)), SLOT(updateSerialPorts(int)));
    connect(ui->pushButtonConnectSerial, SIGNAL(pressed()), SLOT(comPortOpen()));


    ui->widgetPlot->addGraph();
    ui->widgetPlot->graph(0)->setName("Data");

    ui->widgetPlot->graph(0)->setData(sampleVec, dataVec);
    ui->widgetPlot->graph(0)->setPen(QPen(Qt::red));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::comPortOpen()
{
    if (ui->pushButtonConnectSerial->isChecked())
    {
        if (!m_pComPort->isOpen())
        {
            QStringList qsl = ui->comboBoxSerialPorts->currentText().split(" ");
            m_pComPort->setPortName(qsl.at(0));
            ui->statusbar->showMessage("Connecting to " + qsl.at(0),3000);
            //m_pComPort->setReadBufferSize(1000);
            m_pComPort->open(QIODevice::ReadWrite);
            m_pComPort->setBaudRate(115200);
            m_pComPort->setFlowControl(QSerialPort::NoFlowControl);
            m_pComPort->setStopBits(QSerialPort::OneStop);
            m_pComPort->setDataBits(QSerialPort::Data8);
            m_pComPort->setParity(QSerialPort::NoParity);

            ui->pushButtonConnectSerial->setText("Disconnect");
            m_pComPort->flush();
            connect(m_pComPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        }
    }
    else
    {
        disconnect(m_pComPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        m_pComPort->close();
        ui->statusbar->showMessage("Disconnected",3000);
        ui->pushButtonConnectSerial->setText("Connect");
    }
}

void MainWindow::updateSerialPorts(int item)
{
    ui->comboBoxSerialPorts->clear();
    // Serial port Init
    QSerialPortInfo port;
    m_pComPort = new QSerialPort(this);
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    foreach (port, ports)
    {
        ui->comboBoxSerialPorts->addItem(port.portName() + "  (" + port.description() + ")");
        //qDebug() << "port name:"       << info.portName;
        //qDebug() << "friendly name:"   << info.friendName;
        //qDebug() << "physical name:"   << info.physName;
        //qDebug() << "enumerator name:" << info.enumName;
        //qDebug() << "vendor ID:"       << info.vendorID;
        //qDebug() << "product ID:"      << info.productID;
        //qDebug() << "===================================";
    }
    int width = ui->comboBoxSerialPorts->minimumSizeHint().width();
    ui->comboBoxSerialPorts->setMinimumWidth(width);
    ui->comboBoxSerialPorts->addItem("Update");
    ui->comboBoxSerialPorts->setCurrentIndex(item);
}

void MainWindow::sendData()
{
    QObject* pushButton = sender();

}

void MainWindow::onReadyRead()
{
    static QByteArray dataArray;
    char dataChar;
    while (m_pComPort->bytesAvailable())
    {
        m_pComPort->read(&dataChar, 1);
        if ((dataChar != '\n') && (dataChar != '\r'))
        {
            dataArray.append(dataChar);
        }
        else
        {
            qDebug() << QString::fromStdString(dataArray.toStdString());
            QStringList qsl = QString::fromStdString(dataArray.toStdString()).split(' ');
            qDebug() << qsl.length();


//            dataVec.clear();
//            sampleVec.clear();
            for (int i = 0; i < (qsl.length()-4); i++)
            {
                bool ok;
                dataVec.append(qsl.at(i).toDouble(&ok));
                sampleVec.append((double)i);
           }

            //ui->widgetPlot->graph(0)->setLineStyle(QCPGraph::lsStepCenter);
            //ui->widgetPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
            //ui->widgetPlot->graph(0)->data().clear();
            ui->widgetPlot->removeGraph(0);
            ui->widgetPlot->addGraph();
            ui->widgetPlot->graph(0)->setData(sampleVec, dataVec);
            ui->widgetPlot->rescaleAxes(true);
            ui->widgetPlot->replot();

            dataArray.clear();
        }
    }
}
