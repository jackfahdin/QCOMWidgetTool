#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QObject>


class SerialPortManager : public QObject
{
    Q_OBJECT

public:
    SerialPortManager();

    void GetSerialPortList();
    bool OpenSerialPort(QString portName,
                        QSerialPort::BaudRate baudRate,
                        QSerialPort::DataBits bit,
                        QSerialPort::Parity parity,
                        QSerialPort::StopBits stop,
                        QSerialPort::FlowControl control);
    void CloseSerialPort();
    void WriteData(const QByteArray &data);
    QByteArray ReadData();
    void SetupDefaultSerialPortSettings();

private :
    QSerialPort *m_serialPort;


signals:
    void ReadyRead();
    void GettingSerialPortList(QStringList serialPortList);
};

#endif // SERIALPORTMANAGER_H
