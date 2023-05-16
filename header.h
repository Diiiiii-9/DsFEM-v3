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

//    qreal mFactor=1.0;

//protected:
//    void wheelEvent(QWheelEvent *event) Q_DECL_OVERRIDE
//    {
//        chart()->zoomReset();

//        mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

//        QRectF rect = chart()->plotArea();
//        QPointF c = chart()->plotArea().center();
//        rect.setWidth(mFactor*rect.width());
//        rect.moveCenter(c);
//        chart()->zoomIn(rect);

//        QChartView::wheelEvent(event);
//    }
};

#endif // HEADER_H
