#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QMouseEvent>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
// координаты 
struct Point 
{
    int x;
    int y;
    
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    QVector<Point> contourPoints;
    bool closed = false;
    int gridStep = 50;
    QPoint origin = QPoint(400,400);
    Point screenToLogical(const QPoint &pos) const;
    
    
    
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
