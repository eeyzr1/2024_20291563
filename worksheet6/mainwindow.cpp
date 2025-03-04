#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ModelPart.h"
#include "ModelPartList.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <functional>

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
        QString Colour = "R";
        QString visible("true");
        ModelPart* childItem = new ModelPart({name, visible});
        rootItem->appendChild(childItem);
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString colour("R");
            QString visible("true");
            ModelPart* childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }
    }

    //WS6 Ex10
    ui->treeView->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* actionItemOptions = new QAction(tr("Item Options"), this);
    ui->treeView->addAction(actionItemOptions);


    // Connect the tree view click signal to the slot
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleSaveButton);

    //connect statusbar with message
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    // Connect the action signal to the slot
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::on_actionOpenFile_triggered);
    connect(actionItemOptions, &QAction::triggered, this, &MainWindow::on_actionItemOptions_triggered);

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

void MainWindow::handleOptionDialog() {//WS6 Ex9
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString inputText = dialog.getInputText();
        emit statusUpdateMessage(QString("Dialog accepted. Input: ") + inputText, 5000);
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"), 5000);
    }
}

void MainWindow::on_actionItemOptions_triggered() {
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to edit.");
        return;
    }

    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    if (!selectedPart) {
        return;
    }

    OptionDialog dialog(this);
    dialog.setWindowTitle("Edit Item Properties");

    // Set initial values in dialog
    dialog.setInputText(selectedPart->data(0).toString());  // Name
    dialog.setRGB(selectedPart->getColourR(), selectedPart->getColourG(), selectedPart->getColourB()); // Colour
    dialog.setItemVisible(selectedPart->visible()); // Visibility

    if (dialog.exec() == QDialog::Accepted) {
        QString newName = dialog.getInputText();
        int newR = dialog.getR();
        int newG = dialog.getG();
        int newB = dialog.getB();
        bool newVisibility = dialog.isItemVisible();

        QModelIndex visibilityIndex = index.sibling(index.row(), 2); // Get the visibility column index

        // Update ModelPart
        selectedPart->set(0, newName);
        selectedPart->setColour(newR, newG, newB);
        selectedPart->setVisible(newVisibility); // Update visibility in model

        // Manually trigger `setData()` for visibility update
        ui->treeView->model()->setData(visibilityIndex, newVisibility ? Qt::Checked : Qt::Unchecked, Qt::CheckStateRole);

        // Force UI to refresh
        emit statusUpdateMessage("Updated item: " + newName +
                                     " (Colour: " + QString("%1,%2,%3").arg(newR).arg(newG).arg(newB) +
                                     ", Visible: " + (newVisibility ? "Yes" : "No") + ")", 5000);
        ui->treeView->update();
    }
}



void MainWindow::saveTreeToFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "File Error", "Could not open file for writing.");
        return;
    }

    QTextStream out(&file);
    ModelPart* root = partList->getRootItem();

    // Recursive function to save items
    std::function<void(ModelPart*, int)> savePart = [&](ModelPart* part, int depth) {
        out << QString(depth * 2, ' ') << part->data(0).toString() << "," << part->data(1).toString() << "\n";
        for (int i = 0; i < part->childCount(); i++) {
            savePart(part->child(i), depth + 1);
        }
    };

    for (int i = 0; i < root->childCount(); i++) {
        savePart(root->child(i), 0);
    }

    file.close();
    emit statusUpdateMessage("Tree saved to " + filePath, 5000);
}

void MainWindow::handleSaveButton() {
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Tree"), "", tr("Text Files (*.txt)"));
    if (!filePath.isEmpty()) {
        saveTreeToFile(filePath); // Now we have a valid file path
    }
}
