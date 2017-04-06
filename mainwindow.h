#pragma once

#include <QMainWindow>
#include <QPainter>
#include <QPicture>
#include "Screen2.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Screen& scr);
    void paintEvent(QPaintEvent *event);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Screen& screen;
};
