#include "WindowFramelessHelper.h"
#include "WindowFramelessHelper_p.h"

#include <QQuickItem>

// class WindowFramelessHelper

WindowFramelessHelper::WindowFramelessHelper(QObject *parent)
    : QObject(parent)
    , d_ptr(new WindowFramelessHelperPrivate())
{
}

WindowFramelessHelper::~WindowFramelessHelper()
{
}

void WindowFramelessHelper::classBegin()
{
}

void WindowFramelessHelper::componentComplete()
{
    Q_D(WindowFramelessHelper);

    auto obj = parent();
    while (nullptr != obj) {
        if (obj->inherits("QQuickRootItem")) {
            if (auto rootItem = qobject_cast<QQuickItem *>(obj)) {
                if (auto window = rootItem->window()) {
                    d->window = window;
                    d->helper = new NativeWindowHelper(window, d);
                }

                break;
            }
        }

        obj = obj->parent();
    }
}

void WindowFramelessHelper::setTopDraggableMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priDraggableMargins.top()) {
        d->priDraggableMargins.setTop(dm);
        emit topDraggableMarginChanged();
    }
}

void WindowFramelessHelper::setLeftDraggableMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priDraggableMargins.left()) {
        d->priDraggableMargins.setLeft(dm);
        emit leftDraggableMarginChanged();
    }
}

void WindowFramelessHelper::setRightDraggableMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priDraggableMargins.right()) {
        d->priDraggableMargins.setRight(dm);
        emit rightDraggableMarginChanged();
    }
}

void WindowFramelessHelper::setBottomDraggableMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priDraggableMargins.bottom()) {
        d->priDraggableMargins.setBottom(dm);
        emit bottomDraggableMarginChanged();
    }
}

int WindowFramelessHelper::topDraggableMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priDraggableMargins.top();
}

int WindowFramelessHelper::leftDraggableMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priDraggableMargins.left();
}

int WindowFramelessHelper::rightDraggableMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priDraggableMargins.right();
}

int WindowFramelessHelper::bottomDraggableMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priDraggableMargins.bottom();
}

void WindowFramelessHelper::setTopMaximizedMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priMaximizedMargins.top()) {
        d->priMaximizedMargins.setTop(dm);
        emit topMaximizedMarginChanged();
    }
}

void WindowFramelessHelper::setLeftMaximizedMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priMaximizedMargins.left()) {
        d->priMaximizedMargins.setLeft(dm);
        emit leftMaximizedMarginChanged();
    }
}

void WindowFramelessHelper::setRightMaximizedMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priMaximizedMargins.right()) {
        d->priMaximizedMargins.setRight(dm);
        emit rightMaximizedMarginChanged();
    }
}

void WindowFramelessHelper::setBottomMaximizedMargin(int dm)
{
    Q_D(WindowFramelessHelper);

    if (dm != d->priMaximizedMargins.bottom()) {
        d->priMaximizedMargins.setBottom(dm);
        emit bottomMaximizedMarginChanged();
    }
}

int WindowFramelessHelper::topMaximizedMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priMaximizedMargins.top();
}

int WindowFramelessHelper::leftMaximizedMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priMaximizedMargins.left();
}

int WindowFramelessHelper::rightMaximizedMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priMaximizedMargins.right();
}

int WindowFramelessHelper::bottomMaximizedMargin() const
{
    Q_D(const WindowFramelessHelper);

    return d->priMaximizedMargins.bottom();
}

void WindowFramelessHelper::addIncludeItem(QQuickItem *item)
{
    Q_D(WindowFramelessHelper);

    d->includeItems.insert(item);
}

void WindowFramelessHelper::removeIncludeItem(QQuickItem *item)
{
    Q_D(WindowFramelessHelper);

    d->includeItems.remove(item);
}

void WindowFramelessHelper::addExcludeItem(QQuickItem *item)
{
    Q_D(WindowFramelessHelper);

    d->excludeItems.insert(item);
}

void WindowFramelessHelper::removeExcludeItem(QQuickItem *item)
{
    Q_D(WindowFramelessHelper);

    d->excludeItems.remove(item);
}

void WindowFramelessHelper::setTitleBarHeight(int v)
{
    Q_D(WindowFramelessHelper);

    d->titleBarHeight = v;
}

int WindowFramelessHelper::titleBarHeight() const
{
    Q_D(const WindowFramelessHelper);

    return d->titleBarHeight;
}

// class WindowFramelessHelperPrivate

WindowFramelessHelperPrivate::WindowFramelessHelperPrivate()
    : window(nullptr)
    , helper(nullptr)
    , titleBarHeight(0)
{
}

WindowFramelessHelperPrivate::~WindowFramelessHelperPrivate()
{
}

QMargins WindowFramelessHelperPrivate::draggableMargins() const
{
    return priDraggableMargins;
}

QMargins WindowFramelessHelperPrivate::maximizedMargins() const
{
    return priMaximizedMargins;
}

bool WindowFramelessHelperPrivate::hitTest(const QPoint &pos) const
{
    if (!window)
        return false;
    else if (titleBarHeight == 0)
        return false;
    else if ((titleBarHeight > 0)
             && (pos.y() >= titleBarHeight))
        return false;

    auto rootItem = window->contentItem();
    if (nullptr == rootItem) {
        return false;
    }

    int currentIndex = -1;
    QQuickItem *items[32] = {0};
    auto child = rootItem;
    QPointF p = pos;

    while (child && (currentIndex < 31)) {
        items[++currentIndex] = child;
        auto grandchild = child->childAt(p.x(), p.y());
        if (nullptr == grandchild) {
            break;
        }

        p = child->mapToItem(grandchild, p);
        child = grandchild;
    }

    while (currentIndex > 0) {
        child = items[currentIndex];
        --currentIndex;

        if (includeItems.contains(child)) {
            break;
        } else if (excludeItems.contains(child)) {
            return false;
        } else if (rootItem == child) {
            break;
        }
    }

    return true;
}
