#include "serialportmanager.h"

SerialPortManager::SerialPortManager()
    : m_serialPort(new QSerialPort)
{
    SetupDefaultSerialPortSettings();
}

void SerialPortManager::SetupDefaultSerialPortSettings()
{
    //设置默认参数
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);
}

void SerialPortManager::GetSerialPortList()
{
    QStringList serialPortList;
    QString portName;
    QString description;
    QString manufacturer;
    QString serialNumber;
    static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        portName = serialPortInfo.portName();
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        serialPortList << serialPortInfo.portName();
 #if 0
                       << (!description.isEmpty() ? description : blankString);

                       << (!manufacturer.isEmpty() ? manufacturer : blankString)
                       << (!serialNumber.isEmpty() ? serialNumber : blankString)
                       << serialPortInfo.systemLocation()
                       << (serialPortInfo.vendorIdentifier()
                               ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
                       << (serialPortInfo.productIdentifier()
                               ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);
#endif
    }

    Q_EMIT GettingSerialPortList(serialPortList);
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

QByteArray SerialPortManager::ReadData()
{
    QByteArray data = m_serialPort->readAll();
    return data;
}


void SerialPortManager::ReadyRead()
{
    m_serialPort->readyRead();
}
