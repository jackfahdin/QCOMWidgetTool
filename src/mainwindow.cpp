#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "commandlistitem.h"
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setMinimumSize(1000, 920);

    // 添加commandlist
    AddCommandList(20);

    // 初始化串口配置
    InitialSerialPortSettings();

    m_serialPort = new QSerialPort();

    // 打开串口
    connect(ui->pushButtonSerialPortSwitch, &QPushButton::clicked, this, &MainWindow::SerialPortSwitch);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::SendData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::AddCommandList(int index)
{
    // 创建CommandListItem实例并添加到listWidgetCommanList
    for(int i = 0; i < 17; i++) {
        CommandListItem *commandItem = new CommandListItem(this);
        QListWidgetItem *newItem = new QListWidgetItem();
        newItem->setSizeHint(commandItem->sizeHint());
        ui->listWidgetCommanList->addItem(newItem);
        ui->listWidgetCommanList->setItemWidget(newItem, commandItem);
    }

    // 修改每个CommandListItem中的数据
    for (int i = 0; i < ui->listWidgetCommanList->count(); ++i) {
        QListWidgetItem *item = ui->listWidgetCommanList->item(i);
        CommandListItem *commandItem = dynamic_cast<CommandListItem*>(ui->listWidgetCommanList->itemWidget(item));
        if (commandItem) {
            commandItem->getCheckBoxCommandListName()->setText(QString("%1:").arg(i + 1));
            commandItem->getLineEditCommand()->setText("");
            // 根据需要修改其他组件的数据
        }
    }

    return 0;
}

void MainWindow::InitialSerialPortSettings()
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

    ui->comboBoxSerialportName->clear();
    for(const QString &portName : serialPortList) {
        ui->comboBoxSerialportName->addItem(portName);
    }

    // 设置波特率
    ui->comboBoxBaudRate->addItem(QStringLiteral("1200"), QSerialPort::Baud1200);
    ui->comboBoxBaudRate->addItem(QStringLiteral("2400"), QSerialPort::Baud2400);
    ui->comboBoxBaudRate->addItem(QStringLiteral("4800"), QSerialPort::Baud4800);
    ui->comboBoxBaudRate->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->comboBoxBaudRate->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->comboBoxBaudRate->addItem(QStringLiteral("57600"), QSerialPort::Baud57600);
    ui->comboBoxBaudRate->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->comboBoxBaudRate->setCurrentIndex(3);
    // 设置数据位
    ui->comboBoxData->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->comboBoxData->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->comboBoxData->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->comboBoxData->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->comboBoxData->setCurrentIndex(3);
    // 设置奇偶校验位
    ui->comboBoxParity->addItem(QStringLiteral("None"), QSerialPort::NoParity);
    ui->comboBoxParity->addItem(QStringLiteral("Even"), QSerialPort::EvenParity);
    ui->comboBoxParity->addItem(QStringLiteral("Odd"), QSerialPort::OddParity);
    ui->comboBoxParity->addItem(QStringLiteral("Mark"), QSerialPort::MarkParity);
    ui->comboBoxParity->addItem(QStringLiteral("Space"), QSerialPort::SpaceParity);
    // 设置停止位
    ui->comboBoxStop->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->comboBoxStop->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
    ui->comboBoxStop->addItem(QStringLiteral("3"), QSerialPort::OneAndHalfStop);
    // 添加流控
    ui->comboBoxControl->addItem(QStringLiteral("None"), QSerialPort::NoFlowControl);
    ui->comboBoxControl->addItem(QStringLiteral("RTS/CTS"), QSerialPort::HardwareControl);
    ui->comboBoxControl->addItem(QStringLiteral("XON/XOFF"), QSerialPort::SoftwareControl);

    // 禁用发送按钮
    ui->pushButtonSend->setEnabled(false);
}

// 重写resizeEvent
void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    QSize newSize = event->size();
    // qDebug() << "当前窗口大小：" << newSize.width() << " x " << newSize.height();
}

void MainWindow::SerialPortSwitch()
{
    if(ui->pushButtonSerialPortSwitch->text() == tr("OpenSerialPort")) {
        // 获取串口名称
        m_serialPort->setPortName(ui->comboBoxSerialportName->currentText());
        // 获取波特率
        m_serialPort->setBaudRate(static_cast<QSerialPort::BaudRate>(ui->comboBoxBaudRate->currentData().toInt()));
        // 获取数据位
        m_serialPort->setDataBits(static_cast<QSerialPort::DataBits>(ui->comboBoxData->currentData().toInt()));
        // 获取奇偶校验位
        m_serialPort->setParity(static_cast<QSerialPort::Parity>(ui->comboBoxParity->currentData().toInt()));
        // 获取停止位
        m_serialPort->setStopBits(static_cast<QSerialPort::StopBits>(ui->comboBoxStop->currentData().toInt()));
        // 获取流控
        m_serialPort->setFlowControl(static_cast<QSerialPort::FlowControl>(ui->comboBoxControl->currentData().toInt()));

        if(m_serialPort->open(QIODevice::ReadWrite)) {
            // 串口成功打开
            ui->comboBoxSerialportName->setEnabled(false);
            ui->comboBoxBaudRate->setEnabled(false);
            ui->comboBoxData->setEnabled(false);
            ui->comboBoxParity->setEnabled(false);
            ui->comboBoxStop->setEnabled(false);
            ui->comboBoxControl->setEnabled(false);
            ui->pushButtonSend->setEnabled(true);
            ui->pushButtonSerialPortSwitch->setText(tr("CloseSerialPort"));

            connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::ReceiveData);
        }
    } else if(ui->pushButtonSerialPortSwitch->text() == tr("CloseSerialPort")) {
        m_serialPort->close();
        ui->comboBoxSerialportName->setEnabled(true);
        ui->comboBoxBaudRate->setEnabled(true);
        ui->comboBoxData->setEnabled(true);
        ui->comboBoxParity->setEnabled(true);
        ui->comboBoxStop->setEnabled(true);
        ui->comboBoxControl->setEnabled(true);
        ui->pushButtonSend->setEnabled(false);
        ui->pushButtonSerialPortSwitch->setText(tr("OpenSerialPort"));

        disconnect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::ReceiveData);
    }
}

void MainWindow::SendData()
{
    QString sendMsg = ui->textEditInputString->toPlainText();

    if(sendMsg.isEmpty())
        return;

    sendMsg.remove(' ');


    m_serialPort->write(sendMsg.toUtf8());
}

void MainWindow::ReceiveData()
{
    QByteArray receiveBuf = m_serialPort->readAll();
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentTime = currentDateTime.toString("[yyyy-MM-dd hh:mm:ss.zzz]: ");
    QString str = QString::fromUtf8(receiveBuf);

    if(receiveBuf.isEmpty())
        return;

    if (ui->checkBoxShowTime->checkState() == 2) {
        ui->textEditOutput->insertPlainText(currentTime);
    }

    ui->textEditOutput->insertPlainText(str);
    ui->textEditOutput->moveCursor(QTextCursor::End);

    if(ui->checkBoxEnter->checkState() == 2) {
        ui->textEditOutput->insertPlainText("\n");
    }
}
