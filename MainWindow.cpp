#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>
#include <QWindow>
#include <QScreen>
#include <QPainter>

#include "FramelessHelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent, Qt::FramelessWindowHint)
    , ui(new Ui::MainWindow())
{
    ui->setupUi(this);

    setWindowTitle("Qt Widgets Inside");

    auto helper = new FramelessHelper(this);
    helper->setDraggableMargins(3, 3, 3, 3);
    helper->setMaximizedMargins(3, 3, 3, 3);
    helper->setTitleBarHeight(32);

    helper->addExcludeItem(ui->minimizeButton);
    helper->addExcludeItem(ui->maximizeButton);
    helper->addExcludeItem(ui->closeButton);

    connect(ui->minimizeButton, &QPushButton::clicked,
            helper, &FramelessHelper::triggerMinimizeButtonAction);
    connect(ui->maximizeButton, &QPushButton::clicked,
            helper, &FramelessHelper::triggerMaximizeButtonAction);
    connect(ui->closeButton, &QPushButton::clicked,
            helper, &FramelessHelper::triggerCloseButtonAction);

    connect(helper, &FramelessHelper::maximizedChanged,
            this, &MainWindow::updateMaximizeButton);

    ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));

    QTimer::singleShot(100, this, &MainWindow::syncPosition);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateMaximizeButton(bool maximized)
{
    if (maximized) {
        ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button2.png")));
        ui->maximizeButton->setToolTip(tr("Restore"));
    } else {
        ui->maximizeButton->setIcon(QIcon(QStringLiteral(":/res/maximize-button1.png")));
        ui->maximizeButton->setToolTip(tr("Maximize"));
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QImage backgroundImage(QStringLiteral(":/res/background.png"));
    painter.drawImage(contentsRect(), backgroundImage);

    /*
    painter.setPen(Qt::red);
    painter.drawRect(rect().adjusted(0, 0, -1, -1));
    painter.setPen(Qt::blue);
    painter.drawRect(rect().adjusted(4, 4, -5, -5));
    */
}

void MainWindow::syncPosition()
{
    QWindow *window = windowHandle();
    QScreen *screen = window->screen();

    window->setX(screen->availableGeometry().width() / 2 + 10);
}
