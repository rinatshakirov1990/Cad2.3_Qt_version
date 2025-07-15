#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(closed)
        return; // контур замкнут
    
    QPoint clickPos = event->pos();
    
     // переоброзование координат, у меня свои координаты у виджета свои нужноо что бы мои (0.0) были посередине экрана где то
    Point newPoint = screenToLogical(clickPos);
    
    // проверяем не пустой ли вектор, если не пустой то добавляем первую точку
    if (contourPoints.isEmpty()) {
        contourPoints.append({0, 0});
        update(); // перерисовать
        return ;
    }
    //получаем предыдущую точку что бы понимать по какому координату движеться линия
    Point prev = contourPoints.last();
    
    
}
