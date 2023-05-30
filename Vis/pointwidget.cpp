#include "pointwidget.h"
#include "ui_pointwidget.h"
#include <QScatterSeries>
#include <QVBoxLayout>
#include <QListView>
#include <QStandardItemModel>
#include <QToolTip>

PointWidget::PointWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointWidget)
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(infoList);
//    // Adjust stretch factors
//    ui->horizontalLayout->setStretchFactor(chartView_, 2); // Expand horizontally more
//    ui->horizontalLayout->setStretchFactor(graphicsView, 2); // Expand horizontally more
//    ui->horizontalLayout->setStretchFactor(infoList, 1); // Expand horizontally less
}


PointWidget::~PointWidget()
{
    delete ui;
}

//void PointWidget::createChartView() {
//    // Create chart and axis objects
//    auto chart = new QChart();

//    // Add series to chart
//    auto series = new QScatterSeries();
//    series->setMarkerSize(10);

//    // Loop over nodes and add points to series
//    for (int i = 0; i < nodes_.size(); ++i) {
//        const auto& node = nodes_[i];
//        //const auto& displacement = nodesDisplace_[i];
//        series->append(node.x(), node.y());
//    }

//    // Set color gradient based on displacement
//    auto gradient = QLinearGradient(QPointF(0, 0), QPointF(0, 1));
//    gradient.setColorAt(0, Qt::blue);
//    gradient.setColorAt(0.5, Qt::green);
//    gradient.setColorAt(1, Qt::red);
//    series->setBrush(gradient);

//    // Add series to chart and set chart view properties
//    chart->addSeries(series);
//    chart->legend()->hide();
//    chart->createDefaultAxes();
//    chartView_ = new QChartView(chart);
//    chartView_->setRenderHint(QPainter::Antialiasing);

//    // Connect clicked signal of series to slot for handling point selection
//    connect(series, &QScatterSeries::clicked, this, &PointWidget::handlePointSelection);

//    ui->horizontalLayout->addWidget(chartView_);
//}

void PointWidget::createChartView() {
    // Create chart and axis objects
    auto chart = new QChart();

    // Add series to chart
    auto series = new QScatterSeries();
    series->setMarkerSize(10);

    // Loop over nodes and add points to series
    for (int i = 0; i < nodes_.size(); ++i) {
        const auto& node = nodes_[i];
        series->append(node.x(), node.y());
    }

    // Set color gradient based on displacement
    auto gradient = QLinearGradient(QPointF(0, 0), QPointF(0, 1));
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(0.5, Qt::green);
    gradient.setColorAt(1, Qt::red);
    series->setBrush(gradient);

    // Add series to chart
    chart->addSeries(series);

    // Create QGraphicsScene
    QGraphicsScene* scene = new QGraphicsScene();

    // Add lines to the scene
    for (const QVector<int>& tri : tris_) {
        assert(tri.size() == 3); // make sure it's a tri

        for (int i = 0; i < 3; ++i) {
            const auto& node1 = nodes_[tri[i]];
            const auto& node2 = nodes_[tri[(i + 1) % 3]];
            scene->addLine(node1.x(), node1.y(), node2.x(), node2.y(), QPen(Qt::black, 0.5));
        }
    }

    // Create QGraphicsView and set the scene
    graphicsView = new QGraphicsView(scene);
    graphicsView->setRenderHint(QPainter::Antialiasing);

    // Flip the graphics view vertically
    QTransform transform;
    transform.scale(1, -1);
    graphicsView->setTransform(transform);

    // Create a QVBoxLayout to hold both the QChartView and QGraphicsView
    QVBoxLayout* layout = new QVBoxLayout();

    // Create a QWidget to hold the layout
    QWidget* widget = new QWidget();
    widget->setLayout(layout);

    // Set chart view properties
    chart->legend()->hide();
    chart->createDefaultAxes();
    chartView_ = new QChartView(chart);
    chartView_->setRenderHint(QPainter::Antialiasing);

    // Remove and delete the existing widgets
    QLayoutItem* item;
    while ((item = ui->horizontalLayout->takeAt(0)) != nullptr) {
        QWidget* widget = item->widget();
        if (widget) {
            ui->horizontalLayout->removeWidget(widget);
            delete widget;
        }
        delete item;
    }

    // Add the QChartView and QGraphicsView to the layout
    ui->horizontalLayout->addWidget(chartView_);
    ui->horizontalLayout->addWidget(graphicsView);

    // Connect clicked signal of series to slot for handling point selection
    connect(series, &QScatterSeries::clicked, this, &PointWidget::handlePointSelection);
    connect(series, &QScatterSeries::hovered, this, &PointWidget::handlePointHovered);

    // Set size policies
    infoList->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum); // Small size preference
    chartView_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Large size preference
    graphicsView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); // Large size preference

}

void PointWidget::handlePointHovered(const QPointF& point, bool state) {
    Q_UNUSED(state);

    // Get index of hovered point
    auto series = qobject_cast<QScatterSeries*>(sender());
    auto index = series->points().indexOf(point);

    // Output the x and y coordinates of the hovered point
    if (index >= 0 && index < nodes_.size()) {
        const auto& node = nodes_[index];

        QString info = QString("Point %1 - x: %2, y: %3")
                           .arg(index)
                           .arg(node.x())
                           .arg(node.y());

        QToolTip::showText(QCursor::pos(), info);
    }
}


void PointWidget::handlePointSelection(const QPointF& point) {
    // Get index of selected point
    auto series = qobject_cast<QScatterSeries*>(sender());
    auto index = series->points().indexOf(point);

    // Output displacement of selected point
    if (index >= 0 && index < nodesDisplace_.size()) {
        const auto& displacement = nodesDisplace_[index];
        const auto& node = nodes_[index];

        QString info = QString("Selected point %1 has displacement in x: %2 in y: %3")
                           .arg(index)
                           .arg(displacement.x() - node.x())
                           .arg(displacement.y() - node.y());

        QStandardItem *item = new QStandardItem(info);
        model->appendRow(item);

        infoList->setModel(model);

    }
}
