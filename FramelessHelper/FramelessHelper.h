#ifndef FRAMELESSHELPER_H
#define FRAMELESSHELPER_H

#include <QWidget>

class FramelessHelperPrivate;
class FramelessHelper : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FramelessHelper)

public:
    explicit FramelessHelper(QWidget *parent = Q_NULLPTR);
    virtual ~FramelessHelper();

public:
    void setDraggableMargins(int left, int top, int right, int bottom);
    void setMaximizedMargins(int left, int top, int right, int bottom);
public:
    void setDraggableMargins(const QMargins &margins);
    void setMaximizedMargins(const QMargins &margins);
public:
    QMargins draggableMargins() const;
    QMargins maximizedMargins() const;

public:
    void addTitleBar(QWidget *w);
    void removeTitleBar(QWidget *w);
public:
    void setTitleBarHeight(int h);
    int titleBarHeight() const;

protected:
    bool eventFilter(QObject *obj, QEvent *ev) Q_DECL_FINAL;
protected:
    QScopedPointer<FramelessHelperPrivate> d_ptr;
};

#endif // FRAMELESSHELPER_H
