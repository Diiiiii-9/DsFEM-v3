#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <QDialog>

#define COL_NUM 4

namespace Ui {
class MeshDialog;
}

class MeshDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MeshDialog(QWidget *parent = nullptr);
    ~MeshDialog();

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_delete_clicked();

    void on_buttonBox_accepted();

private:
    Ui::MeshDialog *ui;
};

#endif // MESHDIALOG_H
