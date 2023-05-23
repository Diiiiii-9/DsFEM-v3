#include "comparisonwidget.h"
#include <QChart>
#include <QLineSeries>
#include <QChartView>
#include <QVBoxLayout>

ComparisonWidget::ComparisonWidget(QWidget* parent)
    : QWidget(parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::Widget) {
    // Initialize member variables if needed

    resize(700, 600); // Set the width and height according to your preference
}



void ComparisonWidget::setTriangles(const QVector<QPointF>& nodes, const QVector<QVector<int>>& tris, const QVector<QPointF>& nodesDisplace) {
    nodes_ = nodes;
    tris_ = tris;
    nodesDisplace_ = nodesDisplace;

    // Create chart and axis objects
    auto chart = new QChart();


    // Loop over triangles and add lines to series
    for (const QVector<int>& tri : tris_) {
        assert(tri.size() == 3); // Make sure it's a triangle

        // Add series for triangles before the force
        QLineSeries *seriesBefore = new QLineSeries();
        seriesBefore->setColor(Qt::lightGray);

        for (int i = 0; i < 3; ++i) {
            const auto& node1 = nodes_[tri[i]];
            const auto& node2 = nodes_[tri[(i + 1) % 3]];
            seriesBefore->append(node1.x(), node1.y());
            seriesBefore->append(node2.x(), node2.y());
        }
        // Add series to chart
        chart->addSeries(seriesBefore);
    }

    // Loop over triangles and add lines to series
    for (const QVector<int>& tri : tris_) {
        assert(tri.size() == 3); // Make sure it's a triangle

        // Add series for triangles after the force
        QLineSeries *seriesAfter = new QLineSeries();
        seriesAfter->setColor(Qt::black);

        for (int i = 0; i < 3; ++i) {
            const auto& node1 = nodesDisplace_[tri[i]];
            const auto& node2 = nodesDisplace_[tri[(i + 1) % 3]];
            seriesAfter->append(node1.x(), node1.y());
            seriesAfter->append(node2.x(), node2.y());
        }
        // Add series to chart
        chart->addSeries(seriesAfter);
    }

    chart->createDefaultAxes();
    chart->legend()->hide();

    // Create chart view and set the chart
    auto chartView = new QChartView(chart);

    // Create a layout for the widget
    auto layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

