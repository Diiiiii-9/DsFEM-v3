#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Job/job.h"
#include "header.h"
#include "Mesh/calcircle.h"
#include "Mesh/calpoly.h"
#include "Job/boundcondition.h"
#include <QMainWindow>
#include <QLabel>
#include <Model/paintwidget.h>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QXYSeries>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void changeCurrentShape(Shape::Code newShape);
    void changeBoundCondition(int boundFlag);

private slots:
    void on_pushButton_Material_clicked();
    void on_pushButton_Color_clicked();
    void on_actionAbout_me_triggered();
    void on_actionAbout_Qt_triggered();
    void on_toolButton_Rect_clicked();
    void on_toolButton_Line_clicked();
    void on_toolButton_Circle_clicked();
    void on_actionNew_triggered();
    void on_actionOpen_Model_triggered();
    void on_actionSave_Model_triggered();
    void on_actionDelete_triggered();
    void on_pushButton_InputData_clicked();
    void on_pushButton_nodeSetting_clicked();
    void on_pushButton_runMesh_clicked();
    void on_pushButton_runModel_clicked();
    void on_toolButton_fix_clicked();
    void on_toolButton_UpDown_clicked();
    void on_toolButton_LeftRight_clicked();
    void on_pushButton_point_clicked();
    void on_pushButton_distribute_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_job_clicked();

    void on_pushButton_selectPoint_clicked();

private:

    Ui::MainWindow *ui;
    PaintWidget *paintWidget = nullptr;
    BoundCondition *bound = nullptr;
    Job *job = nullptr;
    QLabel *mousePosLabel = nullptr;

    int previousIndex = 0;
    float density = 0;
    float e_modulus = 0;;
    float poisson = 0;;
    std::vector<CalCircle> circlePointVector;   //to store circle
    std::vector<CalPoly> polyPointVector;       //to store line&rect

    void onMouseMoved();

    bool isPointOnLine(Point start, Point end, Point point);
    void writeToMeshFile();
    void writeMaterial(QString inFile);
    void showFinalChart(QString outFile);
    /**
     * 1----Fix
     * 2----Vertical
     * 3----Horizon
     * 4----Point Force
     * 5----Distributed Force
    */
    int boundFlag = 0;

    QGraphicsScene *meshScene = nullptr;
    QGraphicsView *meshView = nullptr;
    ChartView *jobView = nullptr;
    //QChartView *visChartView = nullptr;

    QVector<QPointF> nodes;
    QVector<QVector<int>> tris;
    QVector<QPointF> nodesDisplace;
    QVector<float> misesStress;
};


// Define a custom QLabel subclass that resizes the pixmap when the widget is resized
class ResizablePixmapLabel : public QLabel
{
public:
    ResizablePixmapLabel(QWidget *parent = nullptr) : QLabel(parent) {}

protected:
    // Override the resizeEvent() method to resize the pixmap
    void resizeEvent(QResizeEvent *event) override
    {
        QLabel::resizeEvent(event);
        if (!pixmap().isNull()) {
            setPixmap(pixmap().scaled(event->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
};


#endif // MAINWINDOW_H
