#ifndef NATIVEWINDOWHELPER_H
#define NATIVEWINDOWHELPER_H

#include <QWindow>

class NativeWindowTester;
class NativeWindowHelperPrivate;
class NativeWindowHelper : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NativeWindowHelper)

public:
    NativeWindowHelper(QWindow *window, NativeWindowTester *tester);
    explicit NativeWindowHelper(QWindow *window);
    ~NativeWindowHelper();

public:
    bool nativeEventFilter(void *msg, long *result);
protected:
    bool eventFilter(QObject *obj, QEvent *ev) Q_DECL_FINAL;
protected:
    QScopedPointer<NativeWindowHelperPrivate> d_ptr;
};

#endif // NATIVEWINDOWHELPER_H
