#include "numtoplot.h"
#include "ui_numtoplot.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

#define NUM 5

NumToPlot::NumToPlot(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NumToPlot)
{
    ui->setupUi(this);
}

NumToPlot::~NumToPlot()
{
    delete ui;
}

void NumToPlot::on_pushButton_delete_clicked()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }
}


void NumToPlot::on_pushButton_addRect_clicked()
{
    double x1 = ui->X_TopLeftLineEdit->text().toDouble();
    double y1 = ui->Y_TopLeftLineEdit->text().toDouble();
    double x2 = ui->X_BottomRightLineEdit->text().toDouble();
    double y2 = ui->Y_BottomRightLineEdit->text().toDouble();

    QString data_line = QString("Rect") + " "
            + QString::number(x1) + " "
            + QString::number(y1) + " "
            + QString::number(x2) + " "
            + QString::number(y2);

    QListWidgetItem *item = new QListWidgetItem(data_line);
    ui->listWidget->addItem(item);
}

void NumToPlot::on_pushButton_addpoly_clicked()
{
    // Iterate over the points in the polygon
    int pointCount = polygon.size();
    for (int i = 0; i < pointCount; ++i) {
        const QPointF& startPoint = polygon[i];
        const QPointF& endPoint = polygon[(i + 1) % pointCount];  // Wrap around to the first point for the last segment

        // Construct the line in the desired format
        QString line = QString("Line %1 %2 %3 %4")
                           .arg(startPoint.x())
                           .arg(startPoint.y())
                           .arg(endPoint.x())
                           .arg(endPoint.y());


        QListWidgetItem* item = new QListWidgetItem(line);
        ui->listWidget->addItem(item);
    }
}


void NumToPlot::on_pushButton_addCircle_clicked()
{
    double x1 = ui->X_cenLineEdit->text().toDouble();
    double y1 = ui->Y_cenLineEdit->text().toDouble();
    double r = ui->rLineEdit->text().toDouble();

    QString data_line = QString("Circle") + " "
            + QString::number(x1) + " "
            + QString::number(y1) + " "
            + QString::number(x1+r) + " "
            + QString::number(y1);

    QListWidgetItem *item = new QListWidgetItem(data_line);
    ui->listWidget->addItem(item);
}


void NumToPlot::on_buttonBox_accepted()
{
    // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
    QString fileName = "numToPlot_in.txt";
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(NULL, tr("Warning"), tr("File not saved!"));
        return;
    }

    // 2. 创建一个QtextStream对象来操纵file
    QTextStream out(&file);

    // 3. 通过循环，把数据列表中的每一个对象的各项属性都写入文件中。关闭文件
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QListWidgetItem *item = ui->listWidget->item(i);

        QString str = item->text();
        QStringList list = str.split(" ");

        assert(list.size() == NUM);

        for (int j = 0; j < list.size(); ++j) {
            out << list.at(j) << "\r\n";
        }
    }

    file.close();

    ui->listWidget->clear();
}



void NumToPlot::on_pushButton_addpolyPoint_clicked()
{
    QString pointText = ui->lineEdit_polyPoint->text();  // Get the point text from the QLineEdit
    QStringList coordinates = pointText.split(",");  // Split the text by comma to extract x and y coordinates

    if (coordinates.size() == 2) {
        bool conversionOkX, conversionOkY;
        float x = coordinates[0].toFloat(&conversionOkX);  // Convert the x coordinate to float
        float y = coordinates[1].toFloat(&conversionOkY);  // Convert the y coordinate to float

        if (conversionOkX && conversionOkY) {
            // Add the point to the polygon
            polygon << QPointF(x, y);

            // Update the QLabel to display the updated polygon
            QString polygonStr = "Polygon: ";
            for (const QPointF& point : polygon) {
                polygonStr += QString("(%1, %2) ").arg(point.x()).arg(point.y());
            }
            ui->label_poly->setText(polygonStr);
        }
    }
}


void NumToPlot::on_pushButton_deletepoly_clicked()
{
    // Clear the polygon
    polygon.clear();

    // Clear the QLabel
    ui->label_poly->clear();
}

