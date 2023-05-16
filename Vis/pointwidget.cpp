#include "pointwidget.h"
#include "ui_pointwidget.h"
#include <QScatterSeries>
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>

PointWidget::PointWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointWidget)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(infoList);
}

PointWidget::~PointWidget()
{
    delete ui;
}

void PointWidget::createChartView() {
    // Create chart and axis objects
    auto chart = new QChart();
    chart->createDefaultAxes();

    // Add series to chart
    auto series = new QScatterSeries();
    series->setMarkerSize(10);

    // Loop over nodes and add points to series
    for (int i = 0; i < nodes_.size(); ++i) {
        const auto& node = nodes_[i];
        //const auto& displacement = nodesDisplace_[i];
        series->append(node.x(), node.y());
    }

    // Set color gradient based on displacement
    auto gradient = QLinearGradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(0.5, Qt::green);
    gradient.setColorAt(1, Qt::red);
    series->setBrush(gradient);

    // Add series to chart and set chart view properties
    chart->addSeries(series);
    chart->legend()->hide();
    chartView_ = new QChartView(chart);
    chartView_->setRenderHint(QPainter::Antialiasing);

    // Connect clicked signal of series to slot for handling point selection
    connect(series, &QScatterSeries::clicked, this, &PointWidget::handlePointSelection);

    ui->horizontalLayout->addWidget(chartView_);
}

void PointWidget::handlePointSelection(const QPointF& point) {
    // Get index of selected point
    auto series = qobject_cast<QScatterSeries*>(sender());
    auto index = series->points().indexOf(point);

    // Output displacement of selected point
    if (index >= 0 && index < nodesDisplace_.size()) {
        const auto& displacement = nodesDisplace_[index];
        const auto& node = nodes_[index];

//        qDebug() << "Selected point" << index
//                 << "has displacement"
//                 << "in x:" << displacement.x() - node.x()
//                 << "in y:" << displacement.y() - node.y();

        QString info = QString("Selected point %1 has displacement in x: %2 in y: %3")
                           .arg(index)
                           .arg(displacement.x() - node.x())
                           .arg(displacement.y() - node.y());

        QStandardItem *item = new QStandardItem(info);
        model->appendRow(item);

        infoList->setModel(model);

    }
}
