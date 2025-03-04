#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Initialize ModelPartList
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);

    // Add items to the tree
    ModelPart* rootItem = this->partList->getRootItem();
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");
        ModelPart* childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            ModelPart* childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }
    }
    // Connect the tree view click signal to the slot
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    // Connect the action signal to the slot
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::on_actionOpenFile_triggered);

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
void MainWindow::handleTreeClicked(const QModelIndex &index) {
    // Get a pointer to the item from the index
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    // Retrieve the name string from the internal QVariant data array
    QString text = selectedPart->data(0).toString();
    // Emit a status update message
    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}


void MainWindow::on_actionOpenFile_triggered() {//WS6 Ex8
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\Users\\Zhixing\\2024_20291563\\worksheet6",//it is used\\ instead of \ here.
        tr("STL Files (*.stl);;Text Files (*.txt)")
        );
    emit statusUpdateMessage(QString("selected file for ex8"),0);//just for ex8
    if (!fileName.isEmpty()) {
        emit statusUpdateMessage(QString("Selected file: ") + fileName, 5000); // show filename with 5000ms
    }
}
