#include "material.h"
#include "ui_material.h"
#include <QMessageBox>

Material::Material(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Material)
{
    ui->setupUi(this);
}

Material::~Material()
{
    delete ui;
}

//ini data
void Material::on_buttonBox_accepted()
{
    density = ui->densityLineEdit->text().toDouble();
    e_modulus = ui->youngSModulusLineEdit->text().toDouble();
    poisson = ui->poissonSRatioLineEdit->text().toDouble();

    // yes! there's gravity
    if(ui->radioButton_yesGrav->isChecked()){

    }// no! there's not
    else if(ui->radioButton_noGrav->isChecked()){
        density = 0;
    }
    else{
        QMessageBox::critical(this,"ERROR","Choose one!!");
    }

}

double Material::getDensity() const
{
    return density;
}

double Material::getE_modulus() const
{
    return e_modulus;
}

double Material::getPoisson() const
{
    return poisson;
}



