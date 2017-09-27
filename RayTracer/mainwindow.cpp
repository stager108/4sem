#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(Screen& scr) :
    QMainWindow(nullptr),
    screen(scr),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(scr.getSize());
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    (void) event;
    QPainter painter(this);

    painter.drawImage(0, 0, screen.getQImage());
}

MainWindow::~MainWindow()
{
    delete ui;
}
