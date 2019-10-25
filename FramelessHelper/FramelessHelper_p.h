#ifndef STYLEDHELPER_P_H
#define STYLEDHELPER_P_H

#include <QSet>

#include "FramelessHelper.h"

#include "Kernels/NativeWindowHelper.h"

class FramelessHelperPrivate : public NativeWindowTester
{
public:
    FramelessHelperPrivate();
    virtual ~FramelessHelperPrivate();

public:
    QMargins draggableMargins() const final;
    QMargins maximizedMargins() const final;

    bool hitTest(const QPoint &pos) const final;

public:
    QWidget            *window;
    NativeWindowHelper *helper;
public:
    QMargins priDraggableMargins;
    QMargins priMaximizedMargins;
public:
    QSet<QWidget *> extraTitleBars;
    int             titleBarHeight;
public:
    QSet<QWidget *> includeItems;
    QSet<QWidget *> excludeItems;

public:
    bool maximized;
};

#endif // STYLEDHELPER_P_H
