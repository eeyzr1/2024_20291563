#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
    // Set the correct range for RGB spin boxes
    ui->spinBox_R->setRange(0, 255);
    ui->spinBox_G->setRange(0, 255);
    ui->spinBox_B->setRange(0, 255);
}


OptionDialog::~OptionDialog()
{
    delete ui;
}

QString OptionDialog::getInputText() const
{
    return ui->lineEdit->text();
}

void OptionDialog::setInputText(const QString &text) {
    ui->lineEdit->setText(text);
}

bool OptionDialog::isItemVisible() const {
    return ui->checkBoxVisible->isChecked();
}

void OptionDialog::setItemVisible(bool visible) {
    ui->checkBoxVisible->setChecked(visible);
}

void OptionDialog::setRGB(int R, int G, int B) {
    ui->spinBox_R->setValue(R);
    ui->spinBox_G->setValue(G);
    ui->spinBox_B->setValue(B);
}

int OptionDialog::getR() {
    return ui->spinBox_R->value();
}

int OptionDialog::getG() {
    return ui->spinBox_G->value();
}

int OptionDialog::getB() {
    return ui->spinBox_B->value();
}

