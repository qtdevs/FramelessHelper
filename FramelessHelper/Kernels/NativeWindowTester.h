#ifndef NATIVEWINDOWTESTER_H
#define NATIVEWINDOWTESTER_H

#include <QPoint>
#include <QMargins>

class NativeWindowTester
{
public:
    virtual QMargins draggableMargins() const = 0;
    virtual QMargins maximizedMargins() const = 0;

    virtual bool hitTest(const QPoint &pos) const = 0;
};

#endif // NATIVEWINDOWTESTER_H
