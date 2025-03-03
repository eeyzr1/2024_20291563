#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*Original handleButton
void MainWindow::handleButton() {
    QMessageBox msgBox;
    msgBox.setText("Add button was clicked");
    emit statusUpdateMessage("Button was clicked", 2000); // Emit the signal
    msgBox.exec();

}*/
void MainWindow::handleButton() {
    // This causes MainWindow to emit the signal that will then be
    // received by the status bar’s slot
    emit statusUpdateMessage(QString("Add button was clicked"), 500);//emit the message after each release and close by 500ms
}
