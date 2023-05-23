
#ifndef COMPARISONWIDGET_H
#define COMPARISONWIDGET_H


#include <QWidget>


class ComparisonWidget : public QWidget {
    Q_OBJECT

public:
    explicit ComparisonWidget(QWidget* parent = nullptr);
    void setTriangles(const QVector<QPointF>& nodes, const QVector<QVector<int>>& tris, const QVector<QPointF>& nodesDisplace);

private:
    QVector<QPointF> nodes_;
    QVector<QVector<int>> tris_;
    QVector<QPointF> nodesDisplace_;
};


#endif // COMPARISONWIDGET_H
