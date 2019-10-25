#ifndef NATIVEWINDOWHELPER_H
#define NATIVEWINDOWHELPER_H

#include <QPoint>
#include <QWindow>
#include <QMargins>

class NativeWindowTester
{
public:
    virtual QMargins draggableMargins() const = 0;
    virtual QMargins maximizedMargins() const = 0;

    virtual bool hitTest(const QPoint &pos) const = 0;
};

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
    bool eventFilter(QObject *obj, QEvent *ev) final;
protected:
    QScopedPointer<NativeWindowHelperPrivate> d_ptr;

signals:
    void scaleFactorChanged(qreal factor);
public:
    qreal scaleFactor() const;
};

#endif // NATIVEWINDOWHELPER_H
