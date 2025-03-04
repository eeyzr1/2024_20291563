#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPart.h"
#include "ModelPartList.h"
#include "Optiondialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots://for pushbutton1
    void handleButton();

public slots://for pushbutton2Ex9
    void handleOptionDialog();

public slots://Ex10
    void on_actionItemOptions_triggered();

public slots:
    void saveTreeToFile(const QString &filePath);  // Declare the function

public slots:
    void handleSaveButton(); // Intermediate slot

public slots:
    void handleTreeClicked(const QModelIndex &index);//Ex5 for tree click

public slots://Ex6 for open file
    void on_actionOpenFile_triggered();

signals:
    void statusUpdateMessage(const QString &message, int timeout);

private:
    Ui::MainWindow *ui;

private:
    ModelPartList* partList;//add for declare a pointer to partlist.

};
#endif // MAINWINDOW_H
