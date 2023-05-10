#ifndef NUMTOPLOT_H
#define NUMTOPLOT_H

#include <QDialog>

namespace Ui {
class NumToPlot;
}

class NumToPlot : public QDialog
{
    Q_OBJECT

public:
    explicit NumToPlot(QWidget *parent = nullptr);
    ~NumToPlot();

private slots:
    void on_pushButton_delete_clicked();

    void on_pushButton_addRect_clicked();

    void on_pushButton_addLine_clicked();

    void on_pushButton_addCircle_clicked();

    void on_buttonBox_accepted();

private:
    Ui::NumToPlot *ui;
};

#endif // NUMTOPLOT_H
