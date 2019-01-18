#ifndef NATIVEWINDOWFILTER_H
#define NATIVEWINDOWFILTER_H

#include <QAbstractNativeEventFilter>

class QWindow;
class NativeWindowHelper;

class NativeWindowFilter : public QAbstractNativeEventFilter
{
public:
    static void deliver(QWindow *window, NativeWindowHelper *helper);
protected:
    bool nativeEventFilter(const QByteArray &eventType,
                           void *message, long *result) final;
};

#endif // NATIVEWINDOWFILTER_H
