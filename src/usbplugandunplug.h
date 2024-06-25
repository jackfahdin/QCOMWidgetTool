#ifndef USBPLUGANDUNPLUG_H
#define USBPLUGANDUNPLUG_H

#include <QWidget>
#include <QAbstractNativeEventFilter>
#include <Windows.h>
#include <Dbt.h>

class USBPlugAndUnplug : public QWidget, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit USBPlugAndUnplug(QWidget *parent = nullptr);
    ~USBPlugAndUnplug();

protected:
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;

signals:
    void UsbStatusChange();
};

#endif // USBPLUGANDUNPLUG_H
