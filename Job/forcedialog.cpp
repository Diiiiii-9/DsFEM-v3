#include "forcedialog.h"
#include "ui_forcedialog.h"

ForceDialog::ForceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ForceDialog)
{
    ui->setupUi(this);
}

ForceDialog::~ForceDialog()
{
    delete ui;
}

void ForceDialog::on_buttonBox_accepted()
{
    this->force_x = ui->forceXLineEdit->text().toFloat();
    this->force_y = ui->forceYLineEdit->text().toFloat();
}

float ForceDialog::getForce_x() const
{
    return force_x;
}

float ForceDialog::getForce_y() const
{
    return force_y;
}

