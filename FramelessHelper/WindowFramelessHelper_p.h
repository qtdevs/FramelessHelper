#ifndef QSHARKFRAMELESSHELPER_P_H
#define QSHARKFRAMELESSHELPER_P_H

#include <QSet>
#include <QQuickWindow>

#include "WindowFramelessHelper.h"

#include "Kernels/NativeWindowHelper.h"

class WindowFramelessHelperPrivate : public NativeWindowTester
{
public:
    WindowFramelessHelperPrivate();
    virtual ~WindowFramelessHelperPrivate();

public:
    QMargins draggableMargins() const final;
    QMargins maximizedMargins() const final;

    bool hitTest(const QPoint &pos) const final;

public:
    QQuickWindow       *window;
    NativeWindowHelper *helper;
public:
    QMargins priDraggableMargins;
    QMargins priMaximizedMargins;
public:
    QSet<QQuickItem *> extraTitleBars;
    int                titleBarHeight;
public:
    QSet<QQuickItem *> includeItems;
    QSet<QQuickItem *> excludeItems;
};

#endif // QSHARKFRAMELESSHELPER_P_H
