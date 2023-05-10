#ifndef MATERIAL_H
#define MATERIAL_H

#include <QDialog>

namespace Ui {
class Material;
}

class Material : public QDialog
{
    Q_OBJECT

public:
    explicit Material(QWidget *parent = nullptr);
    ~Material();

    double getDensity() const;
    double getE_modulus() const;
    double getPoisson() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Material *ui;
    double density;
    double e_modulus;
    double poisson;
};

#endif // MATERIAL_H
