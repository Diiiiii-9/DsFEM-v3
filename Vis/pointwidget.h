#ifndef POINTWIDGET_H
#define POINTWIDGET_H

#include <QWidget>
#include <QChartView>
#include <QListView>
#include <QStandardItemModel>

namespace Ui {
class PointWidget;
}

class PointWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PointWidget(QWidget *parent = nullptr);
    ~PointWidget();

    void setMeshData(const QVector<QPointF>& nodes, const QVector<QPointF>& nodesDisplace, const QVector<QVector<int>>& tris) {
        nodes_ = nodes;
        nodesDisplace_ = nodesDisplace;
        tris_ = tris;
    }

    void createChartView();

private:
    Ui::PointWidget *ui;

    QVector<QPointF> nodes_;
    QVector<QPointF> nodesDisplace_;
    QVector<QVector<int>> tris_;

    QChartView* chartView_;


    QListView *infoList = new QListView(this);

    QStandardItemModel *model = new QStandardItemModel(this);

private slots:
    void handlePointSelection(const QPointF& point);
};

#endif // POINTWIDGET_H
