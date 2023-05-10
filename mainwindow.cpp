#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCharts>
#include <QMouseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>

#include <set>
#include <bits/stdc++.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <tuple>
#include <iostream>


#include "Model/paintwidget.h"
#include "Model/material.h"
#include "Model/circle.h"
#include "Model/line.h"
#include "Model/rect.h"
#include "Model/numtoplot.h"
//#include "Model/cgraph.h"

#include "Mesh/meshdialog.h"
#include "Mesh/boundary.h"
#include "Mesh/cal.h"

extern "C"
{
    #include "Mesh/tmesh/include/tmesh/mesh.h"
}

#include "Job/boundcondition.h"
#include "Job/job.h"

#include "Vis/pointdialog.h"

//极小值
#define epsilon 1e-6
#define PI 3.14159265

struct Triangle {
    QPointF center;
    float stress;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    mousePosLabel = new QLabel(this);
//    statusBar()->addPermanentWidget(mousePosLabel);

    paintWidget = new PaintWidget(this);
    bound = new BoundCondition(this);

    ui->horizontalLayout_model->addWidget(paintWidget);

    // 连接 MainWindow发出的changeCurrentShape信号与 paintWidget的setCurrentShape槽
    connect(this, SIGNAL(changeCurrentShape(Shape::Code)), paintWidget, SLOT(setCurrentShape(Shape::Code)));
    // 连接 MainWindow发出的changeBoundCondition信号与 bound的setCurrentBound槽
    connect(this, SIGNAL(changeBoundCondition(int)), bound, SLOT(setCurrentBound(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onMouseMoved()
{
    QPoint pos = QCursor::pos();
    QString text = QString("X: %1, Y: %2").arg(pos.x()).arg(pos.y());
    mousePosLabel->setText(text);
}


//************************** Model Start *******************************

//Show material widget
void MainWindow::on_pushButton_Material_clicked()
{
    Material material;
    material.setModal(true);
    int reply = material.exec();

    //reply == 1 means dialog is accepted.
    if(reply == 1){
        density = material.getDensity();
        e_modulus = material.getE_modulus();
        poisson = material.getPoisson();
    }
    else{

    }

}


//Color picker
void MainWindow::on_pushButton_Color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    QString msg = QString("r: %1, g: %2, b: %3").arg(QString::number(color.red()), QString::number(color.green()), QString::number(color.blue()));
    QMessageBox::information(NULL, "Selected color", msg);
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this,"About Qt");
}


void MainWindow::on_actionAbout_me_triggered()
{
    QMessageBox::information(this,"About Diiiiii","learn more from Github: Diiiiii-9");
}

void MainWindow::on_toolButton_Rect_clicked()
{
    emit changeCurrentShape(Shape::Rect);
}

void MainWindow::on_toolButton_Circle_clicked()
{
    emit changeCurrentShape(Shape::Circle);
}


void MainWindow::on_toolButton_Line_clicked()
{
    emit changeCurrentShape(Shape::Line);
}


void MainWindow::on_actionNew_triggered()
{
    // 1. Ask if he need to save it
    QMessageBox::StandardButton reply = QMessageBox::question(this,
                                        "Save or not?", "Do you want to save the file?",
                                        QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
    // 2. Save the file
        on_actionSave_Model_triggered();

    }else{
    // 3. Don't save it
        qDebug() << "no save now";
    }

    // 4. Clear the list and update
    paintWidget->shapeList.clear();
    paintWidget->update();
}

void MainWindow::on_actionOpen_Model_triggered()
{
    // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("dat Files (*.dat)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(NULL, tr("Warning"), tr("File not opened"));
        return;
    }

    // 2. 清空数据列表
    paintWidget->shapeList.clear();

    // 3. 创建一个QtextStream对象来操纵file
    QTextStream in(&file);

    // 4. 当QtextStream对象没有到末尾时，循环
    while (! in.atEnd())
    {
        // 4.1 取得文件中的shapeName, 存入s中
        QString s;
        in >> s;
        if (s == "")
            break;

        // 4.2 根据文件中shapeName的值，决定创建Shape的哪一种子对象。然后把newShape指向新创建的对象
        Shape * newShape = nullptr;
        if (s == "Circle")
            newShape = new Circle(this->paintWidget);
        else if (s == "Rect")
            newShape = new Rect(this->paintWidget);
        else if (s == "Line")
            newShape = new Line(this->paintWidget);
        else
            QMessageBox::critical(this, "ERROR", "Something wrong with the file. Please check it twice.");

        // 4.3 获取文件中的四个坐标值，存入新创建的Shape的子对象中
        float sx, sy, ex, ey;
        in >> sx >> sy >> ex >> ey;
        newShape->setStart(QPoint(sx, sy));
        newShape->setEnd(QPoint(ex, ey));

        // 4.4 把创建好的Shape的子对象添加到数据列表中
        paintWidget->shapeList.append(newShape);
    }

    // 5. 刷新画布，显示出上一步添加的所有新数据。关闭文件
    paintWidget->update();
    file.close();
}

void MainWindow::on_actionSave_Model_triggered()
{
    // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), ".", tr("dat Files (*.dat)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(NULL, tr("Warning"), tr("File not saved!"));
        return;
    }

    // 2. 创建一个QtextStream对象来操纵file
    QTextStream out(&file);

    // 3. 通过循环，把数据列表中的每一个Shape对象的各项属性都写入文件中。关闭文件
    foreach (Shape *shape, paintWidget->shapeList)    {
        // windows下换行是\r\n，unix下换行是\n，如果在行末只添加\n, 在windows下用记事本打开此文件时会丢失换行符，所以在行末添加\r\n
        out << shape->shapeName         << "\r\n"
            << shape->getStart().x()    << "\r\n"
            << shape->getStart().y()    << "\r\n"
            << shape->getEnd().x()      << "\r\n"
            << shape->getEnd().y()      << "\r\n";
    }
    file.close();
}

void MainWindow::on_actionDelete_triggered()
{
    paintWidget->shapeList.clear();
    paintWidget->update();
    bound = new BoundCondition(this);

}

void MainWindow::on_pushButton_InputData_clicked()
{
    NumToPlot numtoplot;
    numtoplot.setModal(true);
    int reply = numtoplot.exec();

    // 1. If reply == 1.
    if(reply)
    {
        // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
        QString fileName = "numToPlot_in.txt";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(NULL, tr("Warning"), tr("File not opened"));
            return;
        }

        // 2. 可以不用清空数据列表
        //paintWidget->shapeList.clear();

        // 3. 创建一个QtextStream对象来操纵file
        QTextStream in(&file);

        // 4. 当QtextStream对象没有到末尾时，循环
        while (! in.atEnd())
        {
            // 4.1 取得文件中的shapeName, 存入s中
            QString s;
            in >> s;
            if (s == "")
                break;

            // 4.2 根据文件中shapeName的值，决定创建Shape的哪一种子对象。然后把newShape指向新创建的对象
            Shape * newShape = nullptr;
            if (s == "Circle")
                newShape = new Circle(this->paintWidget);
            else if (s == "Rect")
                newShape = new Rect(this->paintWidget);
            else if (s == "Line")
                newShape = new Line(this->paintWidget);
            else
                QMessageBox::critical(this, "ERROR", "Something wrong with the file. Please check it twice.");

            // 4.3 获取文件中的四个坐标值，存入新创建的Shape的子对象中
            float sx, sy, ex, ey;
            in >> sx >> sy >> ex >> ey;
            newShape->setStart(QPoint(sx, sy));
            newShape->setEnd(QPoint(ex, ey));

            // 4.4 把创建好的Shape的子对象添加到数据列表中
            paintWidget->shapeList.append(newShape);
        }

        // 5. 刷新画布，显示出上一步添加的所有新数据。关闭文件
        paintWidget->update();
        file.close();
    }
}

//determine if a point is on line segement
bool MainWindow::isPointOnLine(Point start, Point end, Point point)
{
    float crossproduct = (point.y- start.y) * (end.x - start.x) - (point.x - start.x) * (end.y - start.y);

    // compare versus epsilon for floating point values, or != 0 if using integers
    if (abs(crossproduct) > epsilon)
        return false;

    float dotproduct = (point.x - start.x) * (end.x - start.x) + (point.y - start.y)*(end.y - start.y);
    if (dotproduct < 0)
        return false;

    float squaredlengthba = (end.x - start.x)*(end.x - start.x) + (end.y - start.y)*(end.y - start.y);
    if (dotproduct > squaredlengthba)
        return false;

    return true;
}

//Check if the model is correct
// Too hard to identify
void MainWindow::on_pushButton_runModel_clicked()
{
    // Clear vector
    circlePointVector.clear();
    polyPointVector.clear();


    //to determine if the point is the start point
    bool firstPoint = true;
    pair<float, float> tmpPair;
    CalPoly *newOne = nullptr;
    CalCircle *newCircle = nullptr;


    // 1. store all point info (Line)
    foreach (Shape *shape, paintWidget->shapeList) {
        QPoint start = shape->getStart();
        QPoint end = shape->getEnd();

        if(shape->shapeName == "Line")
        {
            //no the first point
            if(!firstPoint)
            {
                //new starter
                if ((tmpPair.first - start.x()) > epsilon || (tmpPair.first - start.y()) > epsilon )
                {
                    polyPointVector.push_back(*newOne);
                    firstPoint = true;
                }
                else
                {
                    Point point {static_cast<float>(end.x()),static_cast<float>(end.y())};
                    newOne->AddDirectedLine(point);
                    tmpPair = make_pair(end.x(),end.y());
                    continue;
                }

            }

            //for those first point of the poly
            newOne = new CalPoly(start.x(),start.y());
            Point _point {static_cast<float>(start.x()),static_cast<float>(start.y())};
            newOne->AddDirectedLine(_point);
            tmpPair = make_pair(end.x(),end.y());
            firstPoint = false;

        }
        else if(shape->shapeName == "Rect")
        {

            float sx, sy, ex, ey;

            sx = start.x();
            sy = start.y();
            ex = end.x();
            ey = end.y();

            newOne = new CalPoly(start.x(),start.y());
            Point point1 {sx,sy};
            Point point2 {sx,ey};
            Point point3 {ex,ey};
            Point point4 {ex,sy};
            newOne->AddDirectedLine(point1);
            newOne->AddDirectedLine(point2);
            newOne->AddDirectedLine(point3);
            newOne->AddDirectedLine(point4);
            polyPointVector.push_back(*newOne);

        }
        else if(shape->shapeName == "Circle")
        {
            float r = sqrt(pow(start.x()-end.x(), 2) + pow(start.y()-end.y(), 2));
            newCircle = new CalCircle(start.x(),start.y(),r);
            circlePointVector.push_back(*newCircle);
        }
        else
        {
            QMessageBox::critical(this,"ERROR","wrong with modeling");
        }

    }

    // Delete the previous view if it exists
    if (meshView != nullptr)
    {
        delete meshView;
        meshView = nullptr;
    }


    // Create new view
    meshScene = new QGraphicsScene;
    meshView = new QGraphicsView(meshScene);

    // Flip the view vertically
    QTransform transform;
    transform.scale(1, -1);
    meshView->setTransform(transform);

    QPen pen(Qt::black, 2);
    pen.setBrush(Qt::NoBrush);

    for (const CalCircle &circle : circlePointVector) {
        QGraphicsEllipseItem *_circle = new QGraphicsEllipseItem(circle.getX() - circle.getR(),
                                                                 circle.getY() - circle.getR(),
                                                                 circle.getR()*2,
                                                                 circle.getR()*2);
        pen.setColor(Qt::red);
        _circle->setPen(pen);
        _circle->setBrush(Qt::transparent);
        meshScene->addItem(_circle);
    }

    for (const CalPoly &poly : polyPointVector) {
        QPolygonF polygonPoints;
        std::vector<Point> points = poly.getPointList();

        for (Point point : points) {
            polygonPoints << QPointF(point.x, point.y);
        }

        QGraphicsPolygonItem *polygon  = new QGraphicsPolygonItem(polygonPoints);
        pen.setColor(Qt::blue);
        polygon->setPen(pen);
        polygon->setBrush(Qt::transparent);
        meshScene->addItem(polygon);
    }

    ui->horizontalLayout_mesh->addWidget(meshView);

    int currentIndex = ui->tabWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->tabWidget->count(); // Wrap around if at the end
    ui->tabWidget->setCurrentIndex(nextIndex);

}




//************************** Mesh Start *******************************
//special mesh point
void MainWindow::on_pushButton_nodeSetting_clicked()
{
    MeshDialog meshdialog;
    meshdialog.setModal(true);
    int reply = meshdialog.exec();

    vector<Point> point_Vec;

    //if yes
    if(reply == 1)
    {
        // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
        QString fileName = "in_node.txt";
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(NULL, tr("Warning"), tr("File not opened"));
            return;
        }

        // 2. 创建一个QtextStream对象来操纵file
        QTextStream in(&file);

        // 3. 当QtextStream对象没有到末尾时，循环
        while (!in.atEnd())
        {
            QString line = in.readLine();
            QStringList parts = line.split(" ");
            if (parts.size() < 2) {
                qWarning("Invalid line: %s", qPrintable(line));
                continue;
            }
            bool ok1, ok2;
            float x = parts.at(0).toFloat(&ok1);
            float y = parts.at(1).toFloat(&ok2);
            if (!ok1 || !ok2) {
                qWarning("Failed to parse line: %s", qPrintable(line));
                continue;
            }

            Point point {x,y};

            point_Vec.push_back(point);
        }

        vector<Point>::iterator itPoint;
        vector<CalCircle>::iterator itCircle;
        vector<CalPoly>::iterator itPoly;

        for(itPoint = point_Vec.begin(); itPoint < point_Vec.end(); itPoint++)
        {

            bool findShape = false;

            // 1. If Circle
            for(itCircle = circlePointVector.begin(); itCircle < circlePointVector.end() && !findShape; itCircle++)
            {
                float length = sqrt(pow(itCircle->getX()-itPoint->x, 2) + pow(itCircle->getY()-itPoint->y, 2));

                //point is on this circle
                if(abs(itCircle->getR() - length) < epsilon)
                {
                    //add points to the list
                    float arctan = std::atan2(itPoint->x - itCircle->getX(), itPoint->y - itCircle->getY());
                    itCircle->addPoints(arctan);
                    findShape = true;
                }

            }

            // 1. If Line & Rect
            for(itPoly = polyPointVector.begin(); itPoly < polyPointVector.end() && !findShape; itPoly++)
            {
                std::vector<Point> polygon = itPoly->getPointList();

                const int n = polygon.size();

                for (int i = 0, j = n - 1; i < n; j = i++) {
                    const Point& p1 = polygon[j];
                    const Point& p2 = polygon[i];

                    Point tmpPoint = Point {itPoint->x, itPoint->y};

                    if (isPointOnLine(p1, p2, tmpPoint)) {
                        itPoly->AddPointBFThis(i, tmpPoint);
                        findShape = true;
                        break;
                    }
                }

            }

        }

        file.close();
    }

}

void writeBoundToFile(bool isReverse, int codeCounter, vector<Point> tmpPoint, int counter, float edgeRefine, QTextStream &out)
{
    if(!isReverse)
    {
        for (int i = 0; i < tmpPoint.size()-1; ++i) {
            out << codeCounter + i << ", " << codeCounter + i+1 << ", " << counter << ", " << edgeRefine << "\r\n";
        }

        out << codeCounter + tmpPoint.size()-1 << ", " << codeCounter << ", " << counter << ", " << edgeRefine << "\r\n";

    }
    else
    {
        for (int i = tmpPoint.size()-1; i > 0 ; --i) {
            out << codeCounter + i << ", " << codeCounter + i-1 << ", " << counter << ", " << edgeRefine << "\r\n";
        }

        out << codeCounter << ", "<< codeCounter + tmpPoint.size()-1   << ", " << counter << ", " << edgeRefine << "\r\n";
    }


}

void MainWindow::writeToMeshFile()
{
    // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
    QString fileName = "mesh_file.para";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(NULL, tr("Warning"), tr("File not saved!"));
        return;
    }

    // 2. 创建一个QtextStream对象来操纵file
    QTextStream out(&file);

    //******************************* Mesh ***********************************

    float gloabalElementSize = ui->elementSizeLineEdit->text().toFloat();
    int quadElement = 50;
    //Model can be within -10.0, -10.0, 1000.0, 1000.0
    QString boundingBox = "-10.0, -10.0, 1000.0, 1000.0";
    QString sizeFunc = ui->sizeFunctionLineEdit->text();


    out << "Global element size: " << gloabalElementSize << "\r\n"
        << "Number of quadtree elements: " << quadElement << "\r\n"
        << "Mesh bounding box: " << boundingBox << "\r\n";

    if(!sizeFunc.isEmpty())
        out << "Size function: " << sizeFunc << "\r\n";

    out << "\r\n";

    //******************************* Node ***********************************


    vector<Cal> calList;

    out << "Define nodes:" << "\r\n";

    //1. Line & Rect
    for (const CalPoly &poly : polyPointVector) {

        vector<Point> points = poly.getPointList();

        for (Point point : points) {
            out << point.x << ", " << point.y << "\r\n";
        }

        Cal *cal = new Cal();
        cal->setPointsList(points);
        calList.push_back(*cal);

    }
    //2. Circle
    for (CalCircle &circle : circlePointVector) {
//        Point centre = circle.getCentre();
        float x = circle.getX();
        float y = circle.getY();
        float r = circle.getR();

        int numOfCir = 2*PI*r / gloabalElementSize;
        vector<float> points = circle.addNPointsEvenly(numOfCir);

        for (float point : points) {
            out << x + r*cos(point) << ", " << y + r*sin(point) << "\r\n";
        }

        Cal *cal = new Cal();
        cal->setPointsList(circle.getPointsbyPoint());
        calList.push_back(*cal);

    }

    out << "End nodes" << "\r\n\r\n";

    //****************************** BOUNDARIES *********************************

    vector<Boundary> boundaryVector;
    vector<Point> judgePoints;  //store the whole thing

    //1. create judge point
    for(const Cal &cal : calList)
    {
        judgePoints.push_back(cal.findFirstPoint());
    }

    int InterBCount = 0;
    int ExterBCount = 0;


    for (const Cal &cal : calList) {

        vector<Point> _judgePoints = judgePoints;

        vector<Point> points = cal.getPointsList();

        Point _point = points.front();

        for (int i = 0; i < _judgePoints.size(); ++i) {
            Point point = _judgePoints.at(i);
            if(abs(_point.x - point.x) < epsilon && abs(_point.y - point.y) < epsilon){
                _judgePoints.erase(_judgePoints.begin()+i);
                break;
            }
        }

        Boundary *_bound = new Boundary(points);

        /*
         * 0--- no interior either exterior
         * 1--- interior
         * 2--- exterior
         */
        int flag = _bound->boundCondition(_judgePoints);

        if(flag == 1)
            InterBCount++;
        else if(flag == 2)
            ExterBCount++;
        else
            cout << "error with bound" << endl;

        boundaryVector.push_back(*_bound);

    }

    // make sure there's only one exterior boundary
    assert(ExterBCount == 1);

    int incnt = 0;
    int counter = 0;
    int codeCounter = 0;

    double edgeRefine = ui->edgeRefinementLineEdit->text().toDouble();

    for (const Boundary &_bound : boundaryVector) {
        assert(incnt <= InterBCount);
        vector<Point> tmpPoint = _bound.getPoly();
        int direc = _bound.getBound_direction();

        if(_bound.getIsExter())
        {
            out << "Define exterior boundary: 1" << "\r\n";

            if(direc == 1)
                writeBoundToFile(true, codeCounter, tmpPoint, counter, edgeRefine, out);
            else if(direc == 2)
                writeBoundToFile(false, codeCounter, tmpPoint, counter, edgeRefine, out);
            else
                cout << "error with exterior boundary" << endl;

            out << "End exterior boundary" << "\r\n";

            codeCounter += tmpPoint.size();

        }
        else
        {

            out << "Define interior boundary: " << incnt+1 << "\r\n";

            if(direc == 1)  //clockwise
                writeBoundToFile(false, codeCounter, tmpPoint, counter, edgeRefine, out);
            else if(direc == 2) //counter-clockwise
                writeBoundToFile(true, codeCounter, tmpPoint, counter, edgeRefine, out);
            else
                cout << "error with interior boundary" << endl;

            out << "End interior boundary" << "\r\n";

            codeCounter += tmpPoint.size();
            incnt++;
        }

        out << "\r\n";

        counter++;

    }

    file.close();
}

//********************************* Mesh data *************************************


//********************************* Mesh data *************************************




void MainWindow::on_pushButton_runMesh_clicked()
{

    this->writeToMeshFile();

    //QString test_fileName = "test.para";

    QString fileName = "mesh_file.para";

    QByteArray array = fileName.toLocal8Bit();
    char* buffer = array.data();
    mesh(buffer);

    // show nodes and lines on Job widget
    QString out_filename = "mesh_output.txt";

    // Delete the previous view if it exists
    if (jobView != nullptr)
    {
        delete jobView;
        jobView = nullptr;
    }

    jobView = bound->meshPlot(out_filename);
    this->installEventFilter(bound);
    ui->horizontalLayout_job->addWidget(jobView);

    int currentIndex = ui->tabWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->tabWidget->count(); // Wrap around if at the end
    ui->tabWidget->setCurrentIndex(nextIndex);
}




void MainWindow::on_toolButton_fix_clicked()
{
    boundFlag = 1;
    emit changeBoundCondition(boundFlag);
}


void MainWindow::on_toolButton_UpDown_clicked()
{
    boundFlag = 2;
    emit changeBoundCondition(boundFlag);
}


void MainWindow::on_toolButton_LeftRight_clicked()
{
    boundFlag = 3;
    emit changeBoundCondition(boundFlag);
}

void MainWindow::on_pushButton_point_clicked()
{
    boundFlag = 4;
    emit changeBoundCondition(boundFlag);
}


void MainWindow::on_pushButton_distribute_clicked()
{
    boundFlag = 5;
    emit changeBoundCondition(boundFlag);
}


void MainWindow::on_tabWidget_currentChanged(int index)
{
//    QMessageBox msgBox;
//    msgBox.setIcon(QMessageBox::Warning);
//    msgBox.setText("Are you sure you want to switch tabs?");
//    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//    msgBox.setDefaultButton(QMessageBox::Cancel);
//    int reply = msgBox.exec();
//    if (reply == QMessageBox::Cancel) {
//        ui->tabWidget->setCurrentIndex(this->previousIndex);
//    } else {
//        this->previousIndex = index;
//    }
}

void MainWindow::writeMaterial(QString inFile)
{
    QFile file(inFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        qWarning() << "Failed to open file for writing:" << inFile;
        return;
    }

    QTextStream out(&file);

    out << this->poisson << " " << this->e_modulus << Qt::endl;

    file.close();
}


void MainWindow::on_pushButton_job_clicked()
{
    QString inFile = "inJob.txt";
    // write material
    this->writeMaterial(inFile);
    // write bound condition [force and constraint]
    bound->writeToFile(inFile);

    QString outFile = "outJob.txt";

    // Job
    job = new Job();
    job->job_writeToFile(inFile.toStdString().c_str(), outFile.toStdString().c_str());




    // Show in vision
    this->showFinalChart(outFile);

    int currentIndex = ui->tabWidget->currentIndex();
    int nextIndex = (currentIndex + 1) % ui->tabWidget->count(); // Wrap around if at the end
    ui->tabWidget->setCurrentIndex(nextIndex);



}


QColor getColorForMisesStress(float misesStress, float minMisesStress, float maxMisesStress) {

    // Define the colors to use for the minimum and maximum values
    QColor minColor(Qt::blue);
    QColor maxColor(Qt::red);

    // Calculate the color based on the misesStress value
    float valueRange = maxMisesStress - minMisesStress;
    float valuePosition = (misesStress - minMisesStress) / valueRange;
    int red = static_cast<int>((maxColor.red() - minColor.red()) * valuePosition + minColor.red());
    int green = static_cast<int>((maxColor.green() - minColor.green()) * valuePosition + minColor.green());
    int blue = static_cast<int>((maxColor.blue() - minColor.blue()) * valuePosition + minColor.blue());

    // Return the color
    return QColor(red, green, blue);

}


void MainWindow::showFinalChart(QString outFile)
{
    // Open file
    QFile file(outFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file";
        return;
    }

    // Read node displacements and Mises stress
    QTextStream in(&file);

    int N, T;
    N = bound->getNumNodes();
    T = bound->getNumTriangles();

    nodes = bound->getNodes();
    tris = bound->getTris();
    nodesDisplace.resize(N);
    misesStress.resize(T);

    for (int i = 0; i < N; i++) {
        float x, y;
        in >> x >> y;
        nodesDisplace[i] = QPointF(nodes.at(i).x() + x, nodes.at(i).y() + y);
    }

    for (int i = 0; i < T; i++) {
        in >> misesStress[i];
    }

    float minMisesStress = misesStress[0];
    float maxMisesStress = misesStress[0];

    for (int i = 1; i < misesStress.size(); i++) {
        if (misesStress[i] < minMisesStress) {
            minMisesStress = misesStress[i];
        }
        if (misesStress[i] > maxMisesStress) {
            maxMisesStress = misesStress[i];
        }
    }

    //********************************* read finish ************************************//

    QChart *noDisChart = new QChart();
    QChart *disChart = new QChart();


    // Loop through the triangles and create a QPolygonF object for each triangle, with the points
    // of the triangle as vertices. Set the fill color of the polygon based on the stress value.

    for (int i = 0; i < tris.size(); i++) {
        QLineSeries *series = new QLineSeries();
        QLineSeries *disSeries = new QLineSeries();
        QLineSeries *series_zero = new QLineSeries();
        //QPolygonF triangle;
        for (int j = 0; j < 3; j++) {
            QPointF node = nodes[tris[i][j]];
            QPointF disNode = nodesDisplace[tris[i][j]];
            *series << node;
            *disSeries << disNode;
        }
        QColor color = getColorForMisesStress(misesStress[i], minMisesStress, maxMisesStress);

        QAreaSeries *seriesArea = new QAreaSeries(series, series_zero);
        QAreaSeries *disSeriesArea = new QAreaSeries(disSeries, series_zero);

        QPen pen(Qt::black);
        pen.setWidth(1);
        seriesArea->setPen(pen);
        disSeriesArea->setPen(pen);

        seriesArea->setColor(color);
        noDisChart->addSeries(seriesArea);

        disSeriesArea->setColor(color);
        disChart->addSeries(disSeriesArea);
    }

    noDisChart->setTitle("Von Mises Stress without displacement");
    noDisChart->legend()->hide();
    noDisChart->createDefaultAxes();

    disChart->setTitle("Von Mises Stress with displacement");
    disChart->legend()->hide();
    disChart->createDefaultAxes();

    // Create two QChartViews and add them to a QHBoxLayout
    QChartView *noDisView = new QChartView(noDisChart);
    QChartView *disView = new QChartView(disChart);

    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(noDisView);
    splitter->addWidget(disView);


    //********************************** Output Setting start **********************************//

    ui->comboBox_visSet->addItem("No displacement");
    ui->comboBox_visSet->addItem("With displacement");

    connect(ui->comboBox_visSet, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
        if (index == 0) {
            noDisView->show();
            disView->hide();
        } else if (index == 1) {
            disView->show();
            noDisView->hide();
        }
    });

    //********************************** Output Setting end **********************************//



    // ******************************* Create a color bar ***********************************

    // Create a vertical layout
    QVBoxLayout* verticalLayout_label = new QVBoxLayout();
    verticalLayout_label->setAlignment(Qt::AlignCenter);

    // Create a linear gradient that spans the Mises stress range
    QLinearGradient gradient(0, 0, 0, 300);
    gradient.setColorAt(0.0, getColorForMisesStress(minMisesStress, minMisesStress, maxMisesStress));
    gradient.setColorAt(1.0, getColorForMisesStress(maxMisesStress, minMisesStress, maxMisesStress));

    // Create a color bar widget and set the gradient as its background
    QLabel* colorBar = new QLabel();
    colorBar->setFixedSize(30, 300);
    colorBar->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred); // Set size policy
    colorBar->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Window, gradient);
    colorBar->setPalette(palette);

    QLabel* min = new QLabel("MIN:" + QString::number(minMisesStress));
    QLabel* max = new QLabel("MAX:" + QString::number(maxMisesStress));

    verticalLayout_label->addWidget(min);
    verticalLayout_label->addWidget(colorBar);
    verticalLayout_label->addWidget(max);

    // Create a widget and set the layout as its layout
    QWidget* labelWidget = new QWidget();
    labelWidget->setLayout(verticalLayout_label);

    // Add the widget to the splitter
    splitter->addWidget(labelWidget);

    ui->verticalLayout_Vis->addWidget(splitter);

}

void MainWindow::on_pushButton_selectPoint_clicked()
{
    PointDialog pointWidget;
    pointWidget.setModal(true);
    int reply = pointWidget.exec();


    // Set mesh data for PointWidget
    pointWidget.setMeshData(nodes, nodesDisplace, tris);

    pointWidget.createChartView();
}

