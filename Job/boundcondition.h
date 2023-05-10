
#ifndef BOUNDCONDITION_H
#define BOUNDCONDITION_H

#include <QString>
#include <QtCharts>
#include <header.h>

typedef QHash<QXYSeries::PointConfiguration, QVariant> PointConfigurations;

class BoundCondition : public QObject
{

    Q_OBJECT

private:
    int boundFlag = 0;
    ChartView* view = new ChartView();
    // Read the nodes
    QVector<QPointF> nodes;
    QScatterSeries* nodeSeries;
    // Create a rubber band object for selecting
    QRubberBand *rubberBand;
    QPointF selectionStartPoint; // To store the starting point of selection
    QPointF selectionEndPoint; // To store the ending point of selection
    QVector<QLineSeries*> lineVector;
    QVector<QString> tris;
    int numNodes = 0;
    int numTriangles = 0;

    QMetaObject::Connection m_selectInitialPointConnection;
    int m_selectedPointIndex = -1;
    PointConfigurations m_selectedPointConfig;


protected:
    // Handle mouse events in ChartView
    bool eventFilter(QObject* obj, QEvent* event) override;

public slots:
    void setCurrentBound(int s) {
        boundFlag = s;
    }

public:
    void writeToFile(QString inFile);
    BoundCondition(QWidget *parent = 0);
    ChartView *meshPlot(QString filename);
    QVector<QPointF> getNodes() const;
    int getNumNodes() const;
    int getNumTriangles() const;
    QVector<QVector<int>> getTris() const;
};





class HorizonConstraint
{

public:
    HorizonConstraint(QObject *parent = 0);
    void writeBound(int index) const;
//    QColor color = Qt::blue;
};

class VerticalConstraint
{
public:
    VerticalConstraint(QObject *parent = 0);
    void writeBound(int index) const;
//    QColor color = Qt::red;
};

class FixedConstraints
{
public:
    FixedConstraints(QObject *parent = 0);
    void writeBound(int index) const;
//    QColor color = Qt::green;
};

class PointForce
{
public:
    PointForce(QObject *parent = 0);
    void writeBound(int index) const;
//    QColor color = Qt::cyan;
};

class DistributedForce
{

public:
    DistributedForce(QObject *parent = 0);
//    QColor color = Qt::cyan;
    void writeBound(int index1, int index2, float force_x, float force_y);

};

#endif // BOUNDCONDITION_H
