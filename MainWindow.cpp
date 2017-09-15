#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>

#include "FramelessHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow())
    , window(Q_NULLPTR)
{
    ui->setupUi(this);

    auto helper = new FramelessHelper(this);
    helper->setDraggableMargins(3, 3, 3, 3);
    helper->setMaximizedMargins(0, 0, 0, 0);
    helper->setTitleBarHeight(32);

    connect(ui->maximizeButton, &QPushButton::clicked,
            this, &MainWindow::maximizeButtonClicked);
    ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QImage backgroundImage(QStringLiteral(":/res/background.png"));
    painter.drawImage(rect(), backgroundImage);
}

bool MainWindow::event(QEvent *event)
{
    if (event->type() == QEvent::WindowStateChange) {
        if (windowState() & Qt::WindowMaximized) {
            ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button2.png")));
            ui->maximizeButton->setToolTip(tr("Restore"));
        } else {
            ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));
            ui->maximizeButton->setToolTip(tr("Maximize"));
        }
    }

    return QWidget::event(event);
}

void MainWindow::maximizeButtonClicked()
{
    if (windowState() & Qt::WindowMaximized) {
        showNormal();
    } else {
        showMaximized();
    }
}

void MainWindow::on_openWindow_clicked()
{
    if (window)
        window->show();
}
