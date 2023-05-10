#ifndef HEADER_H
#define HEADER_H

#include <QVector>
#include <QChartView>



struct Point {
    float x, y;
};

struct line {
    Point p1, p2;
};

class ChartView : public QChartView {
public:
    ChartView(QWidget* parent = nullptr) : QChartView(parent) {}

//protected:
    void mousePressEvent(QMouseEvent* event) override {
        // Handle the mouse press event here
        event->ignore();
        QChartView::mousePressEvent(event); // Call the base implementation
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        event->ignore();
        QChartView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        event->ignore();
        QChartView::mouseReleaseEvent(event);
    }
};

#endif // HEADER_H
