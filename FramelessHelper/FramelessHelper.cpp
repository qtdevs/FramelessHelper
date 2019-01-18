#include "FramelessHelper.h"
#include "FramelessHelper_p.h"

#include <windows.h>

// class FramelessHelper

FramelessHelper::FramelessHelper(QWidget *parent)
    : QObject(parent)
    , d_ptr(new FramelessHelperPrivate())
{
    Q_D(FramelessHelper);
    d->window = parent;
    Q_CHECK_PTR(parent);

    if (d->window)
        d->window->installEventFilter(this);
}

FramelessHelper::~FramelessHelper()
{
}

void FramelessHelper::setDraggableMargins(int left, int top, int right, int bottom)
{
    Q_D(FramelessHelper);

    d->priDraggableMargins = QMargins(left, top, right, bottom);
}

void FramelessHelper::setMaximizedMargins(int left, int top, int right, int bottom)
{
    Q_D(FramelessHelper);

    d->priMaximizedMargins = QMargins(left, top, right, bottom);
}

void FramelessHelper::setDraggableMargins(const QMargins &margins)
{
    Q_D(FramelessHelper);

    d->priDraggableMargins = margins;
}

void FramelessHelper::setMaximizedMargins(const QMargins &margins)
{
    Q_D(FramelessHelper);

    d->priMaximizedMargins = margins;
}

QMargins FramelessHelper::draggableMargins() const
{
    Q_D(const FramelessHelper);

    return d->priDraggableMargins;
}

QMargins FramelessHelper::maximizedMargins() const
{
    Q_D(const FramelessHelper);

    return d->priMaximizedMargins;
}

void FramelessHelper::addIncludeItem(QWidget *item)
{
    Q_D(FramelessHelper);

    d->includeItems.insert(item);
}

void FramelessHelper::removeIncludeItem(QWidget *item)
{
    Q_D(FramelessHelper);

    d->includeItems.remove(item);
}

void FramelessHelper::addExcludeItem(QWidget *item)
{
    Q_D(FramelessHelper);

    d->excludeItems.insert(item);
}

void FramelessHelper::removeExcludeItem(QWidget *item)
{
    Q_D(FramelessHelper);

    d->excludeItems.remove(item);
}

void FramelessHelper::setTitleBarHeight(int v)
{
    Q_D(FramelessHelper);

    d->titleBarHeight = v;
}

int FramelessHelper::titleBarHeight() const
{
    Q_D(const FramelessHelper);

    return d->titleBarHeight;
}

bool FramelessHelper::eventFilter(QObject *obj, QEvent *ev)
{
    Q_D(FramelessHelper);

    if (ev->type() == QEvent::WinIdChange) {
        if (nullptr == d->helper) {
            auto w = d->window->windowHandle();
            d->helper = new NativeWindowHelper(w, d);
        }
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 9, 0)
    if ((ev->type() == QEvent::Resize) || (ev->type() == QEvent::WindowStateChange)) {
        if (d->window->windowState() & Qt::WindowMaximized) {
            const QMargins &m = d->priMaximizedMargins;
            int r = GetSystemMetrics(SM_CXFRAME) * 2 - m.left() - m.right();
            int b = GetSystemMetrics(SM_CYFRAME) * 2 - m.top() - m.bottom();

            d->window->setContentsMargins(0, 0, r, b);
        } else {
            d->window->setContentsMargins(0 ,0, 0, 0);
        }
    }
#endif

    return QObject::eventFilter(obj, ev);
}

// class FramelessHelperPrivate

FramelessHelperPrivate::FramelessHelperPrivate()
    : window(nullptr)
    , helper(nullptr)
    , titleBarHeight(0)
{
}

FramelessHelperPrivate::~FramelessHelperPrivate()
{
}

QMargins FramelessHelperPrivate::draggableMargins() const
{
    return priDraggableMargins;
}

QMargins FramelessHelperPrivate::maximizedMargins() const
{
    return priMaximizedMargins;
}

bool FramelessHelperPrivate::hitTest(const QPoint &pos) const
{
    if (!window)
        return false;
    else if (titleBarHeight == 0)
        return false;
    else if ((titleBarHeight > 0)
             && (pos.y() >= titleBarHeight))
        return false;

    int currentIndex = -1;
    QWidget *items[32] = {0};
    auto child = window;
    QPoint p = pos;

    while (child && (currentIndex < 31)) {
        items[++currentIndex] = child;
        auto grandchild = child->childAt(p);
        if (nullptr == grandchild) {
            break;
        }

        p = grandchild->mapFrom(child, p);
        child = grandchild;
    }

    while (currentIndex > 0) {
        child = items[currentIndex];
        --currentIndex;

        if (includeItems.contains(child)) {
            break;
        } else if (excludeItems.contains(child)) {
            return false;
        } else if (window == child) {
            break;
        }
    }

    return true;
}
