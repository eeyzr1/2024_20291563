#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();
    QString getInputText() const;

public:
    void setInputText(const QString &text);

public:
    bool isItemVisible() const;
    void setItemVisible(bool visible);

public:
    void setRGB(int R, int G, int B);
    int getR();
    int getG();
    int getB();

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H
