#include "meshdialog.h"
#include "ui_meshdialog.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

MeshDialog::MeshDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MeshDialog)
{
    ui->setupUi(this);
}

MeshDialog::~MeshDialog()
{
    delete ui;
}


void MeshDialog::on_pushButton_delete_clicked()
{
    QList<QListWidgetItem*> items = ui->listWidget->selectedItems();
    foreach(QListWidgetItem * item, items)
    {
        delete ui->listWidget->takeItem(ui->listWidget->row(item));
    }

}


void MeshDialog::on_pushButton_add_clicked()
{

    double x = ui->pointXLineEdit->text().toDouble();
    double y = ui->pointYLineEdit->text().toDouble();

    QString data_line = QString::number(x) + " "
            + QString::number(y);

    QListWidgetItem *item = new QListWidgetItem(data_line);
    ui->listWidget->addItem(item);

}


void MeshDialog::on_buttonBox_accepted()
{
    // 1. 获取文件路径来打开文件，并检验文件是否打开。否则直接return
    QString fileName = "in_node.txt";
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

        assert(list.size() == 2);

        out << list.at(0) << " " << list.at(1) << "\r\n";

    }

    file.close();

    ui->listWidget->clear();
}

