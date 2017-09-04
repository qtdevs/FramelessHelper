#ifndef QSHARKFRAMELESSHELPER_P_H
#define QSHARKFRAMELESSHELPER_P_H

#include <QSet>
#include <QQuickWindow>

#include "WindowFramelessHelper.h"

#include "Kernels/NativeWindowHelper.h"
#include "Kernels/NativeWindowTester.h"

class WindowFramelessHelperPrivate : public NativeWindowTester
{
public:
    WindowFramelessHelperPrivate();
    virtual ~WindowFramelessHelperPrivate();

public:
    QMargins draggableMargins() const Q_DECL_FINAL;
    QMargins maximizedMargins() const Q_DECL_FINAL;

    bool hitTest(const QPoint &pos) const Q_DECL_FINAL;

public:
    QQuickWindow       *window;
    NativeWindowHelper *helper;
public:
    QMargins priDraggableMargins;
    QMargins priMaximizedMargins;
public:
    QSet<QQuickItem *> extraTitleBars;
    int                titleBarHeight;
};

#endif // QSHARKFRAMELESSHELPER_P_H
