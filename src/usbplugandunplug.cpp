#include "usbplugandunplug.h"
#include <QApplication>

USBPlugAndUnplug::USBPlugAndUnplug(QWidget *parent)
    : QWidget{parent}
{}

USBPlugAndUnplug::~USBPlugAndUnplug()
{}

bool USBPlugAndUnplug::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);
    int msgType = msg->message;
    if (msgType == WM_DEVICECHANGE)
    {
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            qDebug() << "Detected USB device insertion.";
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            qDebug() << "Detected USB device removal.";
            break;
        case DBT_DEVNODES_CHANGED:
            qDebug() << "Detected USB device status change.";
            emit UsbStatusChange();
            break;
        default:
            break;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}
