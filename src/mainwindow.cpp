#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "commandlistitem.h"
#include <QResizeEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

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
    m_usbPlugAndUnplug = new USBPlugAndUnplug();
    qApp->installNativeEventFilter(m_usbPlugAndUnplug);
    // 打开串口
    connect(ui->pushButtonSerialPortSwitch, &QPushButton::clicked, this, &MainWindow::SerialPortSwitch);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &MainWindow::SendATData);
    connect(ui->pushButtonClearInfo, &QPushButton::clicked, this, &MainWindow::ClearData);
    connect(ui->pushButtonSaveLog, &QPushButton::clicked, this, &MainWindow::SaveReceiveData);

    connect(m_usbPlugAndUnplug, &USBPlugAndUnplug::UsbSerialPortArrival, this, &MainWindow::RefreshSerialPortName);
    connect(m_usbPlugAndUnplug, &USBPlugAndUnplug::UsbSerialPortRemoveCompleft, this, &MainWindow::RefreshSerialPortName);

    customTextEdit = new CustomTextEdit();
    customTextEdit->setStyleSheet("color: rgb(0, 0, 255)");
    customTextEdit->setFont(QFont("Microsoft YaHei UI", 11));
    customTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    customTextEdit->setMaximumHeight(80);
    ui->horizontalLayout_11->replaceWidget(ui->textEditInputString, customTextEdit);
    delete ui->textEditInputString;
    ui->textEditInputString = customTextEdit;

    connect(customTextEdit, &CustomTextEdit::enterPressed, this, &MainWindow::SendATData);
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

void MainWindow::RefreshSerialPortName()
{
    QString serialPortName;
    QString portName;
    QString description;
    QString manufacturer;
    QString serialNumber;
    static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

    ui->comboBoxSerialportName->clear();

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &serialPortInfo : serialPortInfos) {
        portName = serialPortInfo.portName();
        description = serialPortInfo.description();
        manufacturer = serialPortInfo.manufacturer();
        serialNumber = serialPortInfo.serialNumber();
        serialPortName = serialPortInfo.portName() + " (" +
                         (!description.isEmpty() ? description : blankString) + ")";
        ui->comboBoxSerialportName->addItem(serialPortName);
#if 0
                       << (!manufacturer.isEmpty() ? manufacturer : blankString)
                       << (!serialNumber.isEmpty() ? serialNumber : blankString)
                       << serialPortInfo.systemLocation()
                       << (serialPortInfo.vendorIdentifier()
                               ? QString::number(serialPortInfo.vendorIdentifier(), 16) : blankString)
                       << (serialPortInfo.productIdentifier()
                               ? QString::number(serialPortInfo.productIdentifier(), 16) : blankString);
#endif
    }
}

void MainWindow::InitialSerialPortSettings()
{
    RefreshSerialPortName();
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

    SerialPortLogOutput("初始化串口配置\n", "info");
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
        const auto serialPortInfos = QSerialPortInfo::availablePorts();

        // 获取串口名称
        if (!serialPortInfos.isEmpty())
            m_serialPort->setPort(serialPortInfos[ui->comboBoxSerialportName->currentIndex()]);
        // m_serialPort->setPort(QSerialPortInfo::availablePorts()[ui->comboBoxSerialportName->currentIndex()]);
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

            connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::ReceiveLineData);

            SerialPortLogOutput("打开串口" + ui->comboBoxSerialportName->currentText() + "\n", "info");
        } else {
            SerialPortLogOutput("打开串口" + ui->comboBoxSerialportName->currentText() + "失败,串口可能不存在或被占用\n", "error");
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

        disconnect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::ReceiveLineData);

        SerialPortLogOutput("关闭串口" + ui->comboBoxSerialportName->currentText() + "\n", "info");
    }
}

void MainWindow::SendStrData()
{
    QString sendMsg = ui->textEditInputString->toPlainText();

    if(sendMsg.isEmpty())
        return;

    sendMsg.remove(' ');


    m_serialPort->write(sendMsg.toUtf8());
}

void MainWindow::SendATData()
{
    QByteArray byteArray = ui->textEditInputString->toPlainText().toLatin1();
    int ret = 0;

    if(byteArray.isEmpty())
        return;

    ret = m_serialPort->write(byteArray + "\r");
    if(ret < 0)
        SerialPortLogOutput("串口" + ui->comboBoxSerialportName->currentText() + "可能未打开\n", "info");
}

void MainWindow::CheckForEnterKey()
{
    QString text = ui->textEditInputString->toPlainText();
    if (!text.isEmpty() && text.endsWith('\n')) {
        // 文本以回车符结束，执行 SendATData 操作
        SendATData();
        // 清空文本编辑框
        ui->textEditInputString->clear();
    }
}

void MainWindow::ReceiveAllData()
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

    if(ui->checkBoxShowRN->checkState() == 2) {
        str += "\\r\\n";
    }

    if(ui->checkBoxShowHex->checkState() == 2) {
        QString hexString;
        for(auto i = str.begin(); i != str.end(); ++i) {
            QChar c = *i;
            hexString += QString("%1 ").arg(static_cast<quint8>(c.unicode()), 2, 16, QChar('0'));
        }
        ui->textEditOutput->insertPlainText(hexString.trimmed());
    } else {
        ui->textEditOutput->insertPlainText(str);
    }

    ui->textEditOutput->moveCursor(QTextCursor::End);

    if(ui->checkBoxEnter->checkState() == 2) {
        ui->textEditOutput->insertPlainText("\n");
    }
}

void MainWindow::ReceiveLineData()
{
    QByteArray receiveBuf;
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentTime = currentDateTime.toString("[yyyy-MM-dd hh:mm:ss.zzz]: ");

    while (m_serialPort->canReadLine()) {
        receiveBuf = m_serialPort->readLine();
        QString str = QString::fromUtf8(receiveBuf);

        if (ui->checkBoxShowTime->checkState() == 2) {
            ui->textEditOutput->insertPlainText(currentTime);
        }

        if(ui->checkBoxShowRN->checkState() == 2) {
            str += "\\r\\n";
        }

        if(ui->checkBoxShowHex->checkState() == 2) {
            QString hexString;
            for(auto i = str.begin(); i != str.end(); ++i) {
                QChar c = *i;
                hexString += QString("%1 ").arg(static_cast<quint8>(c.unicode()), 2, 16, QChar('0'));
            }
            ui->textEditOutput->insertPlainText(hexString.trimmed());
        } else {
            ui->textEditOutput->insertPlainText(str);
        }

        if(ui->checkBoxEnter->checkState() == 2) {
            ui->textEditOutput->insertPlainText("\n");
        }

        ui->textEditOutput->moveCursor(QTextCursor::End);
    }
    ui->textEditOutput->moveCursor(QTextCursor::End);
}

void MainWindow::ClearData()
{
    ui->textEditOutput->clear();
    ui->textEditSerialPortSettingOutput->clear();
    // ui->textEditInputString->clear();
}

void MainWindow::SaveReceiveData()
{
    QString saveLogPath = QFileDialog::getSaveFileName(this, NULL, qAppName() + "_" +
                            QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt",
                                                       "文本文件(*.txt);;所有文件(*.*)");

    if(saveLogPath.isEmpty())
        return;

    QString messageAll = ui->textEditOutput->toPlainText();

    QFile file(saveLogPath);

    if(file.open(QIODevice::WriteOnly)) {
        file.write(messageAll.toUtf8());
        file.close();
    } else {
        // 失败
        SerialPortLogOutput("接收文件保存失败\n", "error");
    }
}

void MainWindow::SerialPortLogOutput(QString str, QString level)
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentTime = currentDateTime.toString("[yyyy-MM-dd hh:mm:ss.zzz]: ");

    QTextCharFormat format;

    if(level == tr("info")) {
        format.setForeground(Qt::black); // 绿色
    } else if (level == tr("warn")) {
        format.setForeground(Qt::yellow); // 黄色
    } else if(level == tr("error")) {
        format.setForeground(Qt::red); // 红色
    } else {
        format.setForeground(Qt::black);
    }

    QTextCursor cursor = ui->textEditSerialPortSettingOutput->textCursor();
    cursor.movePosition(QTextCursor::End); // 移动到文本末尾

    if (ui->checkBoxShowTime->checkState() == 2) {
        cursor.insertText(currentTime, format); // 插入时间戳并应用格式
    }

    cursor.insertText(str, format); // 插入文本并应用格式
    ui->textEditSerialPortSettingOutput->setTextCursor(cursor); // 更新文本光标
}
