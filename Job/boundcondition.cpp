#include "boundcondition.h"
#include "forcedialog.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QRect>

struct PConstraint {
    int index;
    int type;
};

struct PForce {
    int index;
    float force_x;
    float force_y;
};

// Define the map to hold the elements
std::map<int, PForce> pointForceMap;
std::map<int, PConstraint> pointConstMap;

//极小值
#define epsilon 1e-6

BoundCondition::BoundCondition(QWidget* parent) : QObject(parent)
{
    nodeSeries = new QScatterSeries();

    // Connect the pointHovered signal of the chart to a slot that updates the tooltip
    QObject::connect(nodeSeries, &QXYSeries::hovered, [=](const QPointF& point, bool state) {
        if (state) {
            // If the mouse is hovering over a point, update the tooltip to show its x and y values
            view->setToolTip(QString("X: %1, Y: %2").arg(point.x()).arg(point.y()));
        }
    });

    // Get the position of selected point
    QObject::connect(nodeSeries, &QXYSeries::clicked, nodeSeries, [&](const QPointF &point) {
        int index = nodeSeries->points().indexOf(point);

//        nodeSeries->deselectAllPoints();
//        nodeSeries->selectPoint(index);
//        m_selectedPointIndex = index;
//        m_selectedPointConfig = nodeSeries->pointConfiguration(index);
//        PointConfigurations config = nodeSeries->pointConfiguration(index);

        switch (boundFlag) {
        case 3:
        {
            HorizonConstraint *newBound = new HorizonConstraint();
            newBound->writeBound(index);
            QMessageBox::information(nullptr, "Horizon Constraint Set", "HorizonConstraint has been set for index " + QString::number(index) + ".");

//            m_selectedPointConfig[QXYSeries::PointConfiguration::Color] = QVariant::fromValue(Qt::yellow);
//            nodeSeries->setPointConfiguration(m_selectedPointIndex, m_selectedPointConfig);

            break;
        }
        case 2:
        {
            VerticalConstraint *newBound = new VerticalConstraint();
            newBound->writeBound(index);
            QMessageBox::information(nullptr, "Vertical Constraint Set", "VerticalConstraint has been set for index " + QString::number(index) + ".");


//            m_selectedPointConfig[QXYSeries::PointConfiguration::Color] = QVariant::fromValue(Qt::blue);
//            nodeSeries->setPointConfiguration(m_selectedPointIndex, m_selectedPointConfig);

            break;
        }
        case 1:
        {
            FixedConstraints *newBound = new FixedConstraints();
            newBound->writeBound(index);
            QMessageBox::information(nullptr, "Fixed Constraint Set", "FixedConstraint has been set for index " + QString::number(index) + ".");


//            m_selectedPointConfig[QXYSeries::PointConfiguration::Color] = QVariant::fromValue(Qt::red);
//            nodeSeries->setPointConfiguration(m_selectedPointIndex, m_selectedPointConfig);

            break;
        }
        case 4:
        {
            PointForce *newBound = new PointForce();
            newBound->writeBound(index);
            QMessageBox::information(nullptr, "Point Force Set", "PointForce has been set for index " + QString::number(index) + ".");


//            m_selectedPointConfig[QXYSeries::PointConfiguration::Color] = QVariant::fromValue(Qt::green);
//            nodeSeries->setPointConfiguration(m_selectedPointIndex, m_selectedPointConfig);

            break;
        }
        case 5:
        {

//            m_selectedPointConfig[QXYSeries::PointConfiguration::Color] = QVariant::fromValue(Qt::green);
//            nodeSeries->setPointConfiguration(m_selectedPointIndex, m_selectedPointConfig);

            break;
        }
        default:
            break;
        }
    });


    //****************************** box select **********************************
    // Create the rubber band object
    rubberBand = new QRubberBand(QRubberBand::Rectangle, view);
    rubberBand->setStyleSheet("QRubberBand { background-color: rgba(255, 255, 255, 128); }");

    // Install an event filter to handle mouse events
    view->setMouseTracking(true);

}



QVector<QPointF> BoundCondition::getNodes() const
{
    return nodes;
}

int BoundCondition::getNumNodes() const
{
    return numNodes;
}

int BoundCondition::getNumTriangles() const
{
    return numTriangles;
}

QVector<QVector<int>> BoundCondition::getTris() const
{
    QVector<QVector<int>> indexs;

    for (int i = 0; i < tris.size(); i++) {

        QStringList indices = tris[i].split("\t");

        int index1 = indices[0].toInt();
        int index2 = indices[1].toInt();
        int index3 = indices[2].toInt();

        QVector<int> index;
        index << index1 << index2 << index3;
        indexs.append(index);
    }
    return indexs;
}

bool BoundCondition::eventFilter(QObject* obj, QEvent* event)
{
    //qDebug() << "Filter on";

    if (event->type() == QEvent::MouseButtonPress)
    {
//        qDebug() << "MouseButtonPress";
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            // Start selection

//            selectionStartPoint = mouseEvent->pos();
            selectionStartPoint = view->mapFromGlobal(mouseEvent->globalPosition());
            rubberBand->setGeometry(QRect(selectionStartPoint.toPoint(), QSize()));
            rubberBand->show();
            return true;
        }
    }
    else if (event->type() == QEvent::MouseMove)
    {
//        qDebug() << "MouseMove";
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (rubberBand->isVisible())
        {
            // Update selection
            rubberBand->setGeometry(QRect(selectionStartPoint.toPoint(), view->mapFromGlobal(mouseEvent->globalPosition()).toPoint()).normalized());
            return true;
        }
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
//        qDebug() << "MouseButtonRelease";
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            // End selection
            selectionEndPoint = view->mapFromGlobal(mouseEvent->globalPosition());
            rubberBand->hide();

            // If the mode is not distribute force
            if(boundFlag != 5)
                return true;


            // Determine selection rectangle
            QChart* chart = view->chart(); // Get a pointer to the QChart object displayed in the QChartView
            QRect selectionRect = QRect(chart->mapToValue(selectionStartPoint).toPoint(), chart->mapToValue(selectionEndPoint).toPoint()).normalized();

            QSet<QPair<int, int>> processedLines; // Keep track of the processed lines


            ForceDialog forceDialog;
            forceDialog.setModal(true);
            int reply = forceDialog.exec();

            if(reply != 1)
            {
                qDebug() << "error with void PointForce::writeBound(float x, float y) const";
                return true;
            }


            // Iterate over all lines and check if they are contained within the selection rectangle
            for (QLineSeries* lineSeries : lineVector)
            {
                QVector<QPointF> points = lineSeries->points();

                // make sure there're 4 point every circle. [0-1-2-0]
                assert(points.size() == 4);

                for (int i = 0; i < points.size(); ++i) {
                    int index1 = i;
                    int index2 = (i + 1) % points.size();

                    QPointF p1 = points[index1];
                    QPointF p2 = points[index2];

                    int i1 = nodeSeries->points().indexOf(p1);
                    int i2 = nodeSeries->points().indexOf(p2);

                    if (selectionRect.contains(p1.toPoint()) && selectionRect.contains(p2.toPoint()))
                    {

                        // The line is completely contained within the selection rectangle
                        qreal length = QLineF(p1, p2).length();

                        if(abs(length) > epsilon)
                        {
                            // Check if the line has already been processed
                            QPair<int, int> lineIndices(qMin(i1, i2), qMax(i1, i2));

                            if (!processedLines.contains(lineIndices)) {
                                processedLines.insert(lineIndices);
                                qDebug() << "Selected line with length:" << length << " and endpoint indices:" << i1 << i2;
                                DistributedForce *newBound = new DistributedForce();

                                float force_x = forceDialog.getForce_x() * length / 2;
                                float force_y = forceDialog.getForce_y() * length / 2;

                                newBound->writeBound(i1, i2, force_x, force_y);
                            }
                        }
                    }
                }
            }
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}




HorizonConstraint::HorizonConstraint(QObject *parent)
{

}

VerticalConstraint::VerticalConstraint(QObject *parent)
{

}

FixedConstraints::FixedConstraints(QObject *parent)
{
}

PointForce::PointForce(QObject *parent)
{
}

DistributedForce::DistributedForce(QObject *parent)
{
}

ChartView *BoundCondition::meshPlot(QString filename)
{
       // Open the file for reading
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file" << filename;
        return NULL;
    }



    QTextStream in(&file);
    QString line = in.readLine();
    if (line.startsWith("NODES"))
    {
        QStringList parts = line.split(" ");
        numNodes = parts[1].toInt();
        for (int i = 0; i < numNodes; i++)
        {
            line = in.readLine();
            QStringList coords = line.split("\t");
            QPointF node(coords[0].toDouble(), coords[1].toDouble());
            nodes.append(node);
        }
    }

    // Read the triangles
    line = in.readLine();

    tris.clear();
    if (line.startsWith("TRIANGLES"))
    {
        QStringList parts = line.split(" ");
        numTriangles = parts[1].toInt();

        for (int i = 0; i < numTriangles; i++)
        {
            QLineSeries* edgeSeries = new QLineSeries();

            line = in.readLine();
            tris.append(line);
            QStringList indices = line.split("\t");
            int i1 = indices[0].toInt();
            int i2 = indices[1].toInt();
            int i3 = indices[2].toInt();

            edgeSeries->append(nodes[i1].x(), nodes[i1].y());
            edgeSeries->append(nodes[i2].x(), nodes[i2].y());
            edgeSeries->append(nodes[i3].x(), nodes[i3].y());
            edgeSeries->append(nodes[i1].x(), nodes[i1].y());

            lineVector.push_back(edgeSeries);
        }
    }

    nodeSeries->setMarkerSize(7);
    nodeSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    nodeSeries->setColor(Qt::black);
    for (auto node : nodes)
    {
        nodeSeries->append(node);
    }
    // Create the chart
    QChart* chart = new QChart();

    for (QLineSeries* lineSeries : lineVector) {
        chart->addSeries(lineSeries);
    }
    //chart->addSeries(edgeSeries);
    chart->addSeries(nodeSeries);
    chart->createDefaultAxes();
    chart->legend()->hide();

    // Set the chart margins to leave a blank space on the sides
//    QMargins margins(20, 20, 20, 20);
//    chart->setMargins(margins);

    // Set the chart view properties
    view->setChart(chart);
    view->setRenderHint(QPainter::Antialiasing);


    return view;
}


//1 - movement in the x axis direction is prohibited
//2 - movement in the direction of the y axis is prohibited
//3 - it is forbidden to move in both x and y directions
void HorizonConstraint::writeBound(int index) const
{

    PConstraint newConstraint {index, 1};

    if (pointConstMap.count(newConstraint.index) == 0) {
        // No element with the same index exists, add the new constraint to the container
        pointConstMap[newConstraint.index] = newConstraint;
    } else {
        // An element with the same index exists
        PConstraint& existingConstraint = pointConstMap[newConstraint.index];
        if (existingConstraint.type == newConstraint.type) {
            // The existing constraint has the same type as the new one, do nothing
        } else {
            // The existing constraint has a different type, ask the user if they want to change it
            QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr,
                "Index already exists",
                "A constraint with the same index already exists. Do you want to change its type?",
                QMessageBox::Yes | QMessageBox::No
                );
            if (reply == QMessageBox::Yes) {
                // User wants to change the type, update the existing constraint
                existingConstraint.type = newConstraint.type;
            } else {
                // User doesn't want to change the type, do nothing
            }
        }
    }

}

void VerticalConstraint::writeBound(int index) const
{        
    PConstraint newConstraint {index, 2};

    if (pointConstMap.count(newConstraint.index) == 0) {
        // No element with the same index exists, add the new constraint to the container
        pointConstMap[newConstraint.index] = newConstraint;
    } else {
        // An element with the same index exists
        PConstraint& existingConstraint = pointConstMap[newConstraint.index];
        if (existingConstraint.type == newConstraint.type) {
            // The existing constraint has the same type as the new one, do nothing
        } else {
            // The existing constraint has a different type, ask the user if they want to change it
            QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr,
                "Index already exists",
                "A constraint with the same index already exists. Do you want to change its type?",
                QMessageBox::Yes | QMessageBox::No
                );
            if (reply == QMessageBox::Yes) {
                // User wants to change the type, update the existing constraint
                existingConstraint.type = newConstraint.type;
            } else {
                // User doesn't want to change the type, do nothing
            }
        }
    }
}

void FixedConstraints::writeBound(int index) const
{
    PConstraint newConstraint {index, 3};

    if (pointConstMap.count(newConstraint.index) == 0) {
        // No element with the same index exists, add the new constraint to the container
        pointConstMap[newConstraint.index] = newConstraint;
    } else {
        // An element with the same index exists
        PConstraint& existingConstraint = pointConstMap[newConstraint.index];
        if (existingConstraint.type == newConstraint.type) {
            // The existing constraint has the same type as the new one, do nothing
        } else {
            // The existing constraint has a different type, ask the user if they want to change it
            QMessageBox::StandardButton reply = QMessageBox::question(
                nullptr,
                "Index already exists",
                "A constraint with the same index already exists. Do you want to change its type?",
                QMessageBox::Yes | QMessageBox::No
                );
            if (reply == QMessageBox::Yes) {
                // User wants to change the type, update the existing constraint
                existingConstraint.type = newConstraint.type;
            } else {
                // User doesn't want to change the type, do nothing
            }
        }
    }
}

void PointForce::writeBound(int index) const
{

    ForceDialog forceDialog;
    forceDialog.setModal(true);
    int reply = forceDialog.exec();

    if(reply != 1)
    {
        qDebug() << "error with void PointForce::writeBound(float x, float y) const";
        return;
    }

    float force_x = forceDialog.getForce_x();
    float force_y = forceDialog.getForce_y();

    // Check if an element with the same index already exists
    auto it = pointForceMap.find(index);
    if (it != pointForceMap.end()) {
        // Element with same index already exists, update its force values
        it->second.force_x += force_x;
        it->second.force_y += force_y;
    } else {
        // Element with this index doesn't exist, add a new one
        PForce newPointForce = { index, force_x, force_y };
        pointForceMap.insert({ index, newPointForce });
    }
}

void DistributedForce::writeBound(int index1, int index2, float force_x, float force_y)
{
    QString _string1 = QString::number(index1) + " " + QString::number(force_x) + " " + QString::number(force_y);
    QString _string2 = QString::number(index2) + " " + QString::number(force_x) + " " + QString::number(force_y);

    // Check if an element with the same index already exists
    auto it1 = pointForceMap.find(index1);
    if (it1 != pointForceMap.end()) {
        // Element with same index already exists, update its force values
        it1->second.force_x += force_x;
        it1->second.force_y += force_y;
    } else {
        // Element with this index doesn't exist, add a new one
        PForce newPointForce = { index1, force_x, force_y };
        pointForceMap.insert({ index1, newPointForce });
    }

    // Check if an element with the same index already exists
    auto it2 = pointForceMap.find(index2);
    if (it2 != pointForceMap.end()) {
        // Element with same index already exists, update its force values
        it2->second.force_x += force_x;
        it2->second.force_y += force_y;
    } else {
        // Element with this index doesn't exist, add a new one
        PForce newPointForce = { index2, force_x, force_y };
        pointForceMap.insert({ index2, newPointForce });
    }

}

void BoundCondition::writeToFile(QString inFile)
{
    // Open the file in append mode
    QFile file(inFile);
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "Error: Could not open file for writing.";
        return;
    }

    // Create a text stream to write to the file
    QTextStream out(&file);

    // Write the nodes to the file
    out << nodes.size() << Qt::endl;
    for (const QPointF& point : nodes) {
        out << point.x() << " " << point.y() << Qt::endl;
    }

    // Write the triangles to the file
    out << tris.size() << Qt::endl;
    for (const QString &line : tris) {
        out << line << Qt::endl;
    }

    // Write the constraints to the file
    if (!pointConstMap.empty()) {
        out << pointConstMap.size() << Qt::endl;
        for (auto it = pointConstMap.begin(); it != pointConstMap.end(); ++it) {
            out << it->first << " " << it->second.type << Qt::endl;
        }
    }

    // Write the forces to the file
    if (!pointForceMap.empty()) {
        out << pointForceMap.size() << Qt::endl;
        for (auto it = pointForceMap.begin(); it != pointForceMap.end(); ++it) {
            out << it->first << " " << it->second.force_x << " " << it->second.force_y << Qt::endl;
        }
    }

    // Close the file
    file.close();
}

