#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

QString OptionDialog::getInputText() const
{
    return ui->lineEdit->text();
}
