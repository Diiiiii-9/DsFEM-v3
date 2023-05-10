#ifndef FORCEDIALOG_H
#define FORCEDIALOG_H

#include <QDialog>

namespace Ui {
class ForceDialog;
}

class ForceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ForceDialog(QWidget *parent = nullptr);
    ~ForceDialog();

    float getForce_y() const;

    float getForce_x() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ForceDialog *ui;
    float force_x;
    float force_y;
};

#endif // FORCEDIALOG_H
