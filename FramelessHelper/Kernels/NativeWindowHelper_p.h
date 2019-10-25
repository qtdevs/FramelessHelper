#ifndef NATIVEWINDOWHELPER_P_H
#define NATIVEWINDOWHELPER_P_H

#include "NativeWindowHelper.h"

class NativeWindowHelperPrivate
{
    Q_DECLARE_PUBLIC(NativeWindowHelper)

public:
    NativeWindowHelperPrivate();
    virtual ~NativeWindowHelperPrivate();
protected:
    NativeWindowHelper *q_ptr;

public:
    void updateWindowStyle();
    int hitTest(int x, int y) const;
    bool isMaximized() const;

public:
    QMargins draggableMargins() const;
    QMargins maximizedMargins() const;
public:
    QWindow            *window;
    NativeWindowTester *tester;
public:
    HWND oldWindow;

public:
    QRect availableGeometry() const;
public:
    qreal scaleFactor;
};

#endif // NATIVEWINDOWHELPER_P_H
