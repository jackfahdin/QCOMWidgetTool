#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QKeyEvent>
#include "usbplugandunplug.h"
#include "customtextedit.h"

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

    CustomTextEdit *customTextEdit;

private slots:
    void SerialPortSwitch();
    void SendStrData();
    void SendATData();
    void ReceiveAllData();
    void ReceiveLineData();
    void ClearData();
    void SaveReceiveData();
    void CheckForEnterKey();
};
#endif // MAINWINDOW_H
