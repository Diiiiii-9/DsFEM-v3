#ifndef TRIWIDGET_H
#define TRIWIDGET_H

#include <QWidget>

namespace Ui {
class TriWidget;
}

class TriWidget : public QWidget {
    Q_OBJECT
public:
    explicit TriWidget(QWidget* parent = nullptr);
    ~TriWidget();
    void setTriangles(const QVector<QVector<int>>& triangles);
    void setNodes(const QVector<QPointF>& nodes);
    void setColor(const QVector<QColor>& color);

signals:
    void triangleSelected(int index);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private:
    Ui::TriWidget *ui;
    QVector<QVector<int>> triangles_;
    QVector<QPointF> nodes_;
    QVector<QColor> colors_;
    int selectedTriangleIndex_ = -1;
};


#endif // TRIWIDGET_H
