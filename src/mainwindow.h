#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include "usbplugandunplug.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    int AddCommandList(int index);
    void InitialSerialPortSettings();
    void RefreshSerialPortName();

    void setupCommandListWidget();
    void SetupDefaultSerialPortSettings();

    void SerialPortLogOutput(QString str, QString level);

    QWidget *templateRow;

    QSerialPort *m_serialPort;

    USBPlugAndUnplug *m_usbPlugAndUnplug = nullptr;

private slots:
    void SerialPortSwitch();
    void SendStrData();
    void SendATData();
    void ReceiveData();
    void ClearData();
    void SaveReceiveData();
};
#endif // MAINWINDOW_H
