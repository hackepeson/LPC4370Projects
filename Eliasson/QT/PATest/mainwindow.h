#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort* m_pComPort;
    QVector<double> dataVec;
    QVector<double> sampleVec;

private slots:
    void updateSerialPorts(int item);
    void comPortOpen();
    void sendData();
    void onReadyRead();

};
#endif // MAINWINDOW_H
