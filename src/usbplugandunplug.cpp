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
        PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)msg->lParam;
        switch (msg->wParam) {
        case DBT_DEVICEARRIVAL:
            if(DBT_DEVTYP_PORT == lpdb->dbch_devicetype) {
                Q_EMIT UsbSerialPortArrival();
            }
            break;
        case DBT_DEVICEREMOVECOMPLETE:
            if(DBT_DEVTYP_PORT == lpdb->dbch_devicetype) {
                Q_EMIT UsbSerialPortRemoveCompleft();
            }
            break;
        default:
            break;
        }
    }

    return QWidget::nativeEvent(eventType, message, result);
}
