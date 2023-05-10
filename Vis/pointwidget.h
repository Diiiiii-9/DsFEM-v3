
#ifndef POINTWIDGET_H
#define POINTWIDGET_H


#include <QtCharts>
#include <QWidget>


class PointWidget : public QWidget
{
    Q_OBJECT

private:
    QVector<QPointF> nodes_;
    QVector<QPointF> nodesDisplace_;
    QVector<QVector<int>> tris_;

    QChartView* chartView_;

    void createChartView();

private slots:
    void handlePointSelection(const QPointF& point);

public:
    explicit PointWidget(QWidget *parent = nullptr);

    void setMeshData(const QVector<QPointF>& nodes, const QVector<QPointF>& nodesDisplace, const QVector<QVector<int>>& tris) {
        nodes_ = nodes;
        nodesDisplace_ = nodesDisplace;
        tris_ = tris;
    }

public slots:

signals:

};

#endif // POINTWIDGET_H
