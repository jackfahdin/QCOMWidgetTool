#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>


class SerialPortManager
{
public:
    SerialPortManager();

    QStringList GetSerialPortList();
    bool OpenSerialPort(QString portName,
                        QSerialPort::BaudRate baudRate,
                        QSerialPort::DataBits bit,
                        QSerialPort::Parity parity,
                        QSerialPort::StopBits stop,
                        QSerialPort::FlowControl control);
    void CloseSerialPort();
    void WriteData(const QByteArray &data);
    void ReadData();

private :
    QSerialPort *m_serialPort;
};

#endif // SERIALPORTMANAGER_H
