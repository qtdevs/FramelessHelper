#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) final;
    bool event(QEvent *event) final;
private:
    Ui::MainWindow *ui;

private:
    void syncPosition();
};

#endif // MAINWINDOW_H
