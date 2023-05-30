
#ifndef UNITCONVERSIONWIDGET_H
#define UNITCONVERSIONWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>

class UnitConversionWidget : public QWidget
{
    Q_OBJECT

public:
    UnitConversionWidget(QWidget *parent = nullptr) : QWidget(parent)
    {
        QHBoxLayout *mainLayout = new QHBoxLayout(this);

        // Create the left layout for the "From" part
        QVBoxLayout *leftLayout = new QVBoxLayout();
        mainLayout->addLayout(leftLayout);

        // Create "From" label and input field
        QLabel *fromLabel = new QLabel("From:", this);
        leftLayout->addWidget(fromLabel);

        fromLineEdit = new QLineEdit(this);
        fromLineEdit->setObjectName("fromVal");
        connect(fromLineEdit, &QLineEdit::textChanged, this, &UnitConversionWidget::calculate);
        leftLayout->addWidget(fromLineEdit);

        fromComboBox = new QComboBox(this);
        fromComboBox->setObjectName("calFrom");
        connect(fromComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitConversionWidget::calculate);
        leftLayout->addWidget(fromComboBox);

        // Create the right layout for the "To" part
        QVBoxLayout *rightLayout = new QVBoxLayout();
        mainLayout->addLayout(rightLayout);

        // Create "To" label and input field
        QLabel *toLabel = new QLabel("To:", this);
        rightLayout->addWidget(toLabel);

        toLineEdit = new QLineEdit(this);
        toLineEdit->setObjectName("toVal");
        toLineEdit->setReadOnly(true);
        rightLayout->addWidget(toLineEdit);

        toComboBox = new QComboBox(this);
        toComboBox->setObjectName("calTo");
        connect(toComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &UnitConversionWidget::calculate);
        rightLayout->addWidget(toComboBox);

        // Set up the default values for combo boxes
        QStringList units = {
            "Meter", "Kilometer", "Centimeter", "Millimeter",
            "Micrometer", "Nanometer", "Mile", "Yard",
            "Foot", "Inch", "Light Year"
        };

        fromComboBox->addItems(units);
        toComboBox->addItems(units);

        // Set initial values
        fromComboBox->setCurrentIndex(0);
        toComboBox->setCurrentIndex(1);

        // Set the main layout for the widget
        setLayout(mainLayout);
    }

private slots:
    void calculate()
    {
        QString fromText = fromLineEdit->text();
        double fromValue = fromText.toDouble();

        QString toText;
        double toValue = 0.0;

        if (fromComboBox->currentIndex() != toComboBox->currentIndex())
        {
            double conversionFactor = conversionFactors[fromComboBox->currentIndex()][toComboBox->currentIndex()];
            toValue = fromValue * conversionFactor;
            toText = QString::number(toValue, 'g', 12);
        }

        toLineEdit->setText(toText);

        QString resultText = QString("Result: %1 %2 = %3 %4")
                                 .arg(fromText)
                                 .arg(fromComboBox->currentText())
                                 .arg(toText)
                                 .arg(toComboBox->currentText());

        emit resultChanged(resultText);
    }

signals:
    void resultChanged(const QString &resultText);

private:
    QLineEdit *fromLineEdit;
    QLineEdit *toLineEdit;
    QComboBox *fromComboBox;
    QComboBox *toComboBox;

    const double conversionFactors[11][11] = {
        {1.0, 0.001, 100.0, 1000.0, 1000000.0, 1000000000.0, 0.00062137119, 1.0936133, 3.2808399, 39.3700787, 1.0570008e-16},
        {1000.0, 1.0, 100000.0, 1000000.0, 1000000000.0, 1000000000000.0, 0.62137119, 1093.6133, 3280.8399, 39370.0787, 1.0570008e-13},
        {0.01, 0.00001, 1.0, 10.0, 10000.0, 10000000.0, 0.0000062137, 0.010936133, 0.032808399, 0.393700787, 1.0570008e-18},
        {0.001, 0.000001, 0.1, 1.0, 1000.0, 1000000.0, 0.00000062137, 0.0010936133, 0.0032808399, 0.0393700787, 1.0570008e-19},
        {0.000001, 1.0e-9, 0.0001, 0.001, 1.0, 1000.0, 6.2137119e-10, 1.0936133e-6, 3.2808399e-6, 3.93700787e-5, 1.0570008e-22},
        {1.0e-9, 1.0e-12, 1.0e-7, 1.0e-6, 0.001, 1.0, 6.2137119e-13, 1.0936133e-9, 3.2808399e-9, 3.93700787e-8, 1.0570008e-25},
        {1609.344, 1.609344, 160934.4, 1609344.0, 1609344000.0, 1609344000000.0, 1.0, 1760.0, 5280.0, 63360.0, 1.701079e-13},
        {0.9144, 0.0009144, 91.44, 914.4, 914400.0, 914400000.0, 0.0005681818, 1.0, 3.0, 36.0, 9.665215e-17},
        {0.3048, 0.0003048, 30.48, 304.8, 304800.0, 304800000.0, 0.0001893939, 0.33333333, 1.0, 12.0, 3.2217385e-17},
        {0.0254, 2.54e-5, 2.54, 25.4, 25400.0, 25400000.0, 0.000015783, 0.027777778, 0.083333333, 1.0, 2.6847821e-18},
        {9.461e+15, 9.461e+12, 9.461e+17, 9.461e+18, 9.461e+21, 9.461e+24, 5878625373183.61, 1.0321504e+16, 3.0964512e+16, 3.7157414e+17, 1.0}
    };
};



#endif // UNITCONVERSIONWIDGET_H
