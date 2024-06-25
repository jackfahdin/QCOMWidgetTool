#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

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

    void setupCommandListWidget();
    void SetupDefaultSerialPortSettings();

    QWidget *templateRow;

    QSerialPort *m_serialPort;

private slots:
    // void filterSerialPortList(QStringList serialPortList);
    void SerialPortSwitch();
    void SendData();
    void ReceiveData();
    // void ClearData();
    // void SaveReceiveData();
};
#endif // MAINWINDOW_H
