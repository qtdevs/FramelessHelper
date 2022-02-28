#include "NativeWindowHelper.h"
#include "NativeWindowHelper_p.h"

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>

#include <QGuiApplication>
#include <QScreen>
#include <QEvent>
#include <QtWin>

#include <QOperatingSystemVersion>

#include "NativeWindowFilter.h"

#if defined(__GNUC__)
//我电脑上的mingw报错，说没定义，那咋就给它定义一个
//make mingw happy
#define WM_DPICHANGED       0x02E0
#endif

// class NativeWindowHelper

NativeWindowHelper::NativeWindowHelper(QWindow *window, NativeWindowTester *tester)
    : QObject(window)
    , d_ptr(new NativeWindowHelperPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(NativeWindowHelper);

    Q_CHECK_PTR(window);
    Q_CHECK_PTR(tester);

    d->window = window;
    d->tester = tester;

    if (d->window) {
        d->scaleFactor = d->window->screen()->devicePixelRatio();

        if (d->window->flags() & Qt::FramelessWindowHint) {
            d->window->installEventFilter(this);
            d->updateWindowStyle();
        }
    }
}

NativeWindowHelper::NativeWindowHelper(QWindow *window)
    : QObject(window)
    , d_ptr(new NativeWindowHelperPrivate())
{
    d_ptr->q_ptr = this;

    Q_D(NativeWindowHelper);

    Q_CHECK_PTR(window);
    d->window = window;

    if (d->window) {
        d->scaleFactor = d->window->screen()->devicePixelRatio();

        if (d->window->flags() & Qt::FramelessWindowHint) {
            d->window->installEventFilter(this);
            d->updateWindowStyle();
        }
    }
}

NativeWindowHelper::~NativeWindowHelper()
{
}

bool NativeWindowHelper::nativeEventFilter(void *msg, long *result)
{
    Q_D(NativeWindowHelper);

    Q_CHECK_PTR(d->window);

    LPMSG lpMsg = reinterpret_cast<LPMSG>(msg);
    WPARAM wParam = lpMsg->wParam;
    LPARAM lParam = lpMsg->lParam;

    QPoint pos = QCursor::pos();

    if (WM_NCHITTEST == lpMsg->message) {
        *result = d->hitTest(pos.x(),
                             pos.y());
        return true;
    } else if (WM_NCACTIVATE == lpMsg->message) {
        if (!QtWin::isCompositionEnabled()) {
            if (result) *result = 1;
            return true;
        }
    } else if (WM_NCCALCSIZE == lpMsg->message) {
        if (TRUE == wParam) {
            if (d->isMaximized()) {
                NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(lParam);

                QRect g = d->availableGeometry();
                g.setWidth(g.width() * d->scaleFactor);
                g.setHeight(g.height() * d->scaleFactor);
                QMargins m = d->maximizedMargins();

                params.rgrc[0].top = g.top() - m.top() - 1;
                params.rgrc[0].left = g.left() - m.left() - 1;
                params.rgrc[0].right = g.right() + m.right() + 1;
                params.rgrc[0].bottom = g.bottom() + m.bottom() + 1;
            }

            if (result) *result = 0;
            return true;
        }
    } else if (WM_GETMINMAXINFO == lpMsg->message) {
        LPMINMAXINFO lpMinMaxInfo = reinterpret_cast<LPMINMAXINFO>(lParam);

        QRect g = d->availableGeometry();
        g.setWidth(g.width() * d->scaleFactor);
        g.setHeight(g.height() * d->scaleFactor);
        QMargins m = d->maximizedMargins();

        lpMinMaxInfo->ptMaxPosition.x = - m.left();
        lpMinMaxInfo->ptMaxPosition.y =  - m.top();
        lpMinMaxInfo->ptMaxSize.x = g.right() - g.left() + 1 + m.left() + m.right();
        lpMinMaxInfo->ptMaxSize.y = g.bottom() - g.top() + 1 + m.top() + m.bottom();

        lpMinMaxInfo->ptMinTrackSize.x = d->window->minimumWidth();
        lpMinMaxInfo->ptMinTrackSize.y = d->window->minimumHeight();
        lpMinMaxInfo->ptMaxTrackSize.x = d->window->maximumWidth();
        lpMinMaxInfo->ptMaxTrackSize.y = d->window->maximumHeight();

        if (result) *result = 0;
        return true;
    } else if (WM_NCLBUTTONDBLCLK == lpMsg->message) {
        auto minimumSize = d->window->minimumSize();
        auto maximumSize = d->window->maximumSize();
        if ((minimumSize.width() >= maximumSize.width())
                || (minimumSize.height() >= maximumSize.height())) {
            if (result) *result = 0;
            return true;
        }
    } else if (WM_DPICHANGED == lpMsg->message) {
        qreal old = d->scaleFactor;
        if (HIWORD(wParam) < 144) {
            d->scaleFactor = 1.0;
        } else {
            d->scaleFactor = 2.0;
        }

        if (!qFuzzyCompare(old, d->scaleFactor)) {
            emit scaleFactorChanged(d->scaleFactor);
        }

        auto hWnd = reinterpret_cast<HWND>(d->window->winId());
        auto rect = reinterpret_cast<LPRECT>(lParam);
        SetWindowPos(hWnd,
                     NULL,
                     rect->left,
                     rect->top,
                     rect->right - rect->left,
                     rect->bottom - rect->top,
                     SWP_NOZORDER | SWP_NOACTIVATE);
    } else if (WM_MOVE == lpMsg->message) {
        // Update window pos on move
        auto hWnd = reinterpret_cast<HWND>(d->window->winId());
        RECT rcClient;
        GetWindowRect(hWnd, &rcClient);
        SetWindowPos(hWnd,
                     NULL,
                     rcClient.left,
                     rcClient.top,
                     rcClient.right - rcClient.left,
                     rcClient.bottom - rcClient.top,
                     SWP_FRAMECHANGED);
    }

    return false;
}

bool NativeWindowHelper::eventFilter(QObject *obj, QEvent *ev)
{
    Q_D(NativeWindowHelper);

    if (ev->type() == QEvent::WinIdChange) {
        d->updateWindowStyle();
    } else if (ev->type() == QEvent::Show) {
        d->updateWindowStyle();
    }

    return QObject::eventFilter(obj, ev);
}

qreal NativeWindowHelper::scaleFactor() const
{
    Q_D(const NativeWindowHelper);

    return d->scaleFactor;
}

// class NativeWindowHelperPrivate

NativeWindowHelperPrivate::NativeWindowHelperPrivate()
    : q_ptr(nullptr)
    , window(nullptr)
    , tester(nullptr)
    , oldWindow(NULL)
    , scaleFactor(1.0)
{
}

NativeWindowHelperPrivate::~NativeWindowHelperPrivate()
{
    if (window)
        NativeWindowFilter::deliver(window, nullptr);
}

void NativeWindowHelperPrivate::updateWindowStyle()
{
    Q_Q(NativeWindowHelper);

    Q_CHECK_PTR(window);

    HWND hWnd = reinterpret_cast<HWND>(window->winId());
    if (NULL == hWnd)
        return;
    else if (oldWindow == hWnd) {
        return;
    }
    oldWindow = hWnd;

    NativeWindowFilter::deliver(window, q);

    QOperatingSystemVersion currentVersion = QOperatingSystemVersion::current();
    if (currentVersion < QOperatingSystemVersion::Windows8) {
        return;
    }

    LONG oldStyle = WS_OVERLAPPEDWINDOW | WS_THICKFRAME
            | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;;
    LONG newStyle = WS_POPUP            | WS_THICKFRAME;

    if (QtWin::isCompositionEnabled())
        newStyle |= WS_CAPTION;

    if (window->flags() & Qt::CustomizeWindowHint) {
        if (window->flags() & Qt::WindowSystemMenuHint)
            newStyle |= WS_SYSMENU;
        if (window->flags() & Qt::WindowMinimizeButtonHint)
            newStyle |= WS_MINIMIZEBOX;
        if (window->flags() & Qt::WindowMaximizeButtonHint)
            newStyle |= WS_MAXIMIZEBOX;
    } else {
        newStyle |= WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    }

    LONG currentStyle = GetWindowLong(hWnd, GWL_STYLE);
    SetWindowLong(hWnd, GWL_STYLE, (currentStyle & ~oldStyle) | newStyle);

    SetWindowPos(hWnd, NULL, 0, 0, 0 , 0,
                 SWP_NOOWNERZORDER | SWP_NOZORDER |
                 SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);

    if (QtWin::isCompositionEnabled())
        QtWin::extendFrameIntoClientArea(window, 1, 1, 1, 1);
}

int NativeWindowHelperPrivate::hitTest(int x, int y) const
{
    Q_CHECK_PTR(window);

    QPoint pos_global = QPoint(x, y);

    enum RegionMask {
        Client = 0x0000,
        Top    = 0x0001,
        Left   = 0x0010,
        Right  = 0x0100,
        Bottom = 0x1000,
    };

    QMargins draggableMargins
            = this->draggableMargins();

    int top = draggableMargins.top();
    int left = draggableMargins.left();
    int right = draggableMargins.right();
    int bottom = draggableMargins.bottom();

    if (top <= 0)
        top = GetSystemMetrics(SM_CYFRAME);
    if (left <= 0)
        left = GetSystemMetrics(SM_CXFRAME);
    if (right <= 0)
        right = GetSystemMetrics(SM_CXFRAME);
    if (bottom <= 0)
        bottom = GetSystemMetrics(SM_CYFRAME);

    auto pos = window->mapFromGlobal(pos_global);
    auto wfg = window->geometry();

    auto result =
                (Left *   (pos.x()          <= left)       ) |
                (Right *  (pos.x() + right  >= wfg.width())) |
                (Top *    (pos.y()          <= top)        ) |
                (Bottom * (pos.y() + bottom >= wfg.height()));

    bool wResizable = window->minimumWidth() < window->maximumWidth();
    bool hResizable = window->minimumHeight() < window->maximumHeight();

    switch (result) {
    case Top | Left    : return wResizable && hResizable ? HTTOPLEFT     : HTCLIENT;
    case Top           : return hResizable               ? HTTOP         : HTCLIENT;
    case Top | Right   : return wResizable && hResizable ? HTTOPRIGHT    : HTCLIENT;
    case Right         : return wResizable               ? HTRIGHT       : HTCLIENT;
    case Bottom | Right: return wResizable && hResizable ? HTBOTTOMRIGHT : HTCLIENT;
    case Bottom        : return hResizable               ? HTBOTTOM      : HTCLIENT;
    case Bottom | Left : return wResizable && hResizable ? HTBOTTOMLEFT  : HTCLIENT;
    case Left          : return wResizable               ? HTLEFT        : HTCLIENT;
    }

    return ((nullptr != tester) && !tester->hitTest(pos)) ? HTCLIENT : HTCAPTION;
}

bool NativeWindowHelperPrivate::isMaximized() const
{
    Q_CHECK_PTR(window);

    HWND hWnd = reinterpret_cast<HWND>(window->winId());
    if (NULL == hWnd)
        return false;

    WINDOWPLACEMENT windowPlacement;
    if (!GetWindowPlacement(hWnd, &windowPlacement))
        return false;

    return (SW_MAXIMIZE == windowPlacement.showCmd);
}

QMargins NativeWindowHelperPrivate::draggableMargins() const
{
    return tester ? tester->draggableMargins() * scaleFactor : QMargins();
}

QMargins NativeWindowHelperPrivate::maximizedMargins() const
{
    return tester ? tester->maximizedMargins() * scaleFactor : QMargins();
}

QRect NativeWindowHelperPrivate::availableGeometry() const
{
    // These changes made because MonitorFromWindow doesn't work properly
    // on second monitor. (Blank window on maximize after minimize)
    return window->screen()->availableGeometry();
}
