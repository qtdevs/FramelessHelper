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
    void paintEvent(QPaintEvent *event) Q_DECL_FINAL;
    bool event(QEvent *event) Q_DECL_FINAL;
protected:
    void maximizeButtonClicked();
private slots:
    void on_openWindow_clicked();
private:
    Ui::MainWindow *ui;
public:
    MainWindow *window;
};

#endif // MAINWINDOW_H
