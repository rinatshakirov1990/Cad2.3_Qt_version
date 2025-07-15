#include "mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPolygonF>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setMouseTracking(true); // Чтобы получать события движения мыши без нажатия кнопок
}

void MainWindow::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем линии полилинии
    if(points.size() > 1) {
        for(int i = 0; i < points.size() - 1; ++i) {
            painter.drawLine(points[i], points[i+1]);
        }
        if(closed) {
            painter.drawLine(points.last(), points.first());
        }
    }

    // Рисуем сетку внутри замкнутого полигона
    if(closed && points.size() > 2) {
        QPolygonF polygon(points);
        QRectF boundingRect = polygon.boundingRect();

        painter.save();
        painter.setPen(QPen(Qt::gray, 1, Qt::DotLine));

        // Вертикальные линии сетки
        for (qreal x = boundingRect.left(); x <= boundingRect.right(); x += 10) {
            QVector<QLineF> segments;
            bool insideSegment = false;
            QPointF segmentStart;

            for (qreal y = boundingRect.top(); y <= boundingRect.bottom(); y += 1) {
                QPointF pt(x, y);
                bool inside = polygon.containsPoint(pt, Qt::OddEvenFill);
                if (inside && !insideSegment) {
                    segmentStart = pt;
                    insideSegment = true;
                }
                else if (!inside && insideSegment) {
                    segments.append(QLineF(segmentStart, QPointF(x, y - 1)));
                    insideSegment = false;
                }
            }
            if (insideSegment) {
                segments.append(QLineF(segmentStart, QPointF(x, boundingRect.bottom())));
            }

            for (const QLineF &seg : segments) {
                painter.drawLine(seg);
            }
        }

        // Горизонтальные линии сетки
        for (qreal y = boundingRect.top(); y <= boundingRect.bottom(); y += 10) {
            QVector<QLineF> segments;
            bool insideSegment = false;
            QPointF segmentStart;

            for (qreal x = boundingRect.left(); x <= boundingRect.right(); x += 1) {
                QPointF pt(x, y);
                bool inside = polygon.containsPoint(pt, Qt::OddEvenFill);
                if (inside && !insideSegment) {
                    segmentStart = pt;
                    insideSegment = true;
                }
                else if (!inside && insideSegment) {
                    segments.append(QLineF(segmentStart, QPointF(x - 1, y)));
                    insideSegment = false;
                }
            }
            if (insideSegment) {
                segments.append(QLineF(segmentStart, QPointF(boundingRect.right(), y)));
            }

            for (const QLineF &seg : segments) {
                painter.drawLine(seg);
            }
        }
        painter.restore();
    }

    // Рисуем точки вершин
    painter.setBrush(Qt::red);
    for(const QPointF &pt : points) {
        painter.drawEllipse(pt, 4, 4);
    }

    // Отображаем текущую позицию мыши как синий крестик
    painter.setPen(Qt::blue);
    int size = 5;
    painter.drawLine(mousePos.x()-size, mousePos.y(), mousePos.x()+size, mousePos.y());
    painter.drawLine(mousePos.x(), mousePos.y()-size, mousePos.x(), mousePos.y()+size);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        if(closed) return; // если уже замкнуто — новые точки не добавляем

        QPointF clickedPoint = event->pos();

        // Проверяем, близко ли к первой точке — чтобы замкнуть полилинию
        if(!points.isEmpty()) {
            QPointF first = points.first();
            if(QLineF(first, clickedPoint).length() < 10) {
                closed = true;
                update();
                return;
            }
        }

        points.append(clickedPoint);
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    mousePos = event->pos();
    update();
}
