#ifndef POINTDIALOG_H
#define POINTDIALOG_H

#include <QDialog>
#include <QChartView>

namespace Ui {
class PointDialog;
}

class PointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointDialog(QWidget *parent = nullptr);
    ~PointDialog();

    void setMeshData(const QVector<QPointF>& nodes, const QVector<QPointF>& nodesDisplace, const QVector<QVector<int>>& tris) {
        nodes_ = nodes;
        nodesDisplace_ = nodesDisplace;
        tris_ = tris;
    }

    void createChartView();

private:
    Ui::PointDialog *ui;

    QVector<QPointF> nodes_;
    QVector<QPointF> nodesDisplace_;
    QVector<QVector<int>> tris_;

    QChartView* chartView_;

private slots:
    void handlePointSelection(const QPointF& point);
};

#endif // POINTDIALOG_H
