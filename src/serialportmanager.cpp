#include "serialportmanager.h"

SerialPortManager::SerialPortManager()
    : m_serialPort(new QSerialPort)
{
    //设置默认参数
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
}

QStringList SerialPortManager::GetSerialPortList()
{
    QStringList serialPortList;
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos) {
        serialPortList << info.portName()
                       << info.description();
    }
    return serialPortList;
}

bool SerialPortManager::OpenSerialPort(QString portName,
                                       QSerialPort::BaudRate baudRate,
                                       QSerialPort::DataBits bit,
                                       QSerialPort::Parity parity,
                                       QSerialPort::StopBits stop,
                                       QSerialPort::FlowControl control)
{
    m_serialPort->setPortName(portName);
    if(m_serialPort->open(QIODevice::ReadWrite)) {
        m_serialPort->setBaudRate(baudRate);
        m_serialPort->setDataBits(bit);
        m_serialPort->setParity(parity);
        m_serialPort->setStopBits(stop);
        m_serialPort->setFlowControl(control);
        return true;
    }
    return false;
}

void SerialPortManager::CloseSerialPort()
{
    m_serialPort->close();
}

void SerialPortManager::WriteData(const QByteArray &data)
{
    m_serialPort->write(data);
}

void SerialPortManager::ReadData()
{
    QByteArray data = m_serialPort->readAll();
}
