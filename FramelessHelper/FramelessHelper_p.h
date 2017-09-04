#ifndef STYLEDHELPER_P_H
#define STYLEDHELPER_P_H

#include <QSet>

#include "FramelessHelper.h"

#include "Kernels/NativeWindowHelper.h"
#include "Kernels/NativeWindowTester.h"

class FramelessHelperPrivate : public NativeWindowTester
{
public:
    FramelessHelperPrivate();
    virtual ~FramelessHelperPrivate();

public:
    QMargins draggableMargins() const Q_DECL_FINAL;
    QMargins maximizedMargins() const Q_DECL_FINAL;

    bool hitTest(const QPoint &pos) const Q_DECL_FINAL;

public:
    QWidget            *window;
    NativeWindowHelper *helper;
public:
    QMargins priDraggableMargins;
    QMargins priMaximizedMargins;
public:
    QSet<QWidget *> extraTitleBars;
    int             titleBarHeight;
};

#endif // STYLEDHELPER_P_H
