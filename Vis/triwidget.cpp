#include "triwidget.h"
#include "qevent.h"
#include "ui_triwidget.h"
#include <QPainter>

TriWidget::TriWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TriWidget)
{
    ui->setupUi(this);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

TriWidget::~TriWidget()
{
    delete ui;
}

void TriWidget::setTriangles(const QVector<QVector<int>>& triangles) {
    this->triangles_ = triangles;
    update(); // Trigger a repaint
}

void TriWidget::setColor(const QVector<QColor>& color) {
    this->colors_ = color;
}

void TriWidget::setNodes(const QVector<QPointF>& nodes) {
    this->nodes_ = nodes;
}

void TriWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Flip the coordinate system vertically
    painter.scale(1, -1);
    painter.translate(0, -height());

    // Set the pen's width to a smaller value (e.g., 1.0)
    QPen pen = painter.pen();
    pen.setWidthF(0.1); // Adjust the width as desired
    painter.setPen(pen);

    // Calculate the target rectangle to be shown based on node coordinates
    QPointF minNode = QPointF(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());
    QPointF maxNode = QPointF(std::numeric_limits<qreal>::lowest(), std::numeric_limits<qreal>::lowest());

    for (const QPointF& node : nodes_) {
        minNode.setX(qMin(minNode.x(), node.x()));
        minNode.setY(qMin(minNode.y(), node.y()));
        maxNode.setX(qMax(maxNode.x(), node.x()));
        maxNode.setY(qMax(maxNode.y(), node.y()));
    }

    qreal padding = 10.0;
    QRectF targetRect = QRectF(minNode, maxNode).adjusted(-padding, -padding, padding, padding);

    // Calculate the scaling factors
    qreal scaleX = width() / targetRect.width();
    qreal scaleY = height() / targetRect.height();
    qreal scaleFactor = qMin(scaleX, scaleY);

    // Calculate the translation offset
    QPointF translation = QPointF(-targetRect.x(), -targetRect.y()) * scaleFactor;

    // Apply the transformation to the painter
    painter.translate(translation);
    painter.scale(scaleFactor, scaleFactor);

    for (int i = 0; i < triangles_.size(); ++i) {
        const QVector<int>& tri = triangles_[i];
        if (tri.size() == 3) {
            QPolygonF polygon;
            for (const int index : tri) {
                const auto& node = nodes_[index];
                polygon << QPointF(node.x(), node.y());
            }

            painter.setBrush(colors_[i]);
            painter.drawPolygon(polygon);
        }
    }
}



void TriWidget::mousePressEvent(QMouseEvent* event) {
    QPointF mousePos = event->pos();

    // Flip the y-coordinate
    mousePos.setY(height() - mousePos.y());

    // Calculate the scaling factors and translation offset
    QPointF minNode = QPointF(std::numeric_limits<qreal>::max(), std::numeric_limits<qreal>::max());
    QPointF maxNode = QPointF(std::numeric_limits<qreal>::lowest(), std::numeric_limits<qreal>::lowest());

    for (const QPointF& node : nodes_) {
        minNode.setX(qMin(minNode.x(), node.x()));
        minNode.setY(qMin(minNode.y(), node.y()));
        maxNode.setX(qMax(maxNode.x(), node.x()));
        maxNode.setY(qMax(maxNode.y(), node.y()));
    }

    qreal padding = 10.0;
    QRectF targetRect = QRectF(minNode, maxNode).adjusted(-padding, -padding, padding, padding);

    qreal scaleX = width() / targetRect.width();
    qreal scaleY = height() / targetRect.height();
    qreal scaleFactor = qMin(scaleX, scaleY);

    QPointF translation = QPointF(-targetRect.x(), -targetRect.y()) * scaleFactor;

    // Apply the transformation to the mouse position
    mousePos = (mousePos - translation) / scaleFactor;

    for (int i = 0; i < triangles_.size(); ++i) {
        const QVector<int>& tri = triangles_[i];
        if (tri.size() == 3) {
            QPolygonF polygon;
            for (const int index : tri) {
                const auto& node = nodes_[index];
                polygon << QPointF(node.x(), node.y());
            }

            if (polygon.containsPoint(mousePos, Qt::OddEvenFill)) {
                selectedTriangleIndex_ = i;
                // Emit a signal with the selected triangle index
                emit triangleSelected(i);
                update(); // Trigger a repaint
                return;
            }
        }
    }
}

