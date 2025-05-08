#include "mainwindow.h"
#include "./ui_mainwindow.h" //created by UIC of qt from .ui files
#include "ModelPart.h"
#include "ModelPartList.h"
#include <QFileDialog>//for adding dialog
#include <QtConcurrent>//for concurrent process
#include <QProgressBar>//for showing progress of loading stl model using concurrent
#include <QInputDialog>//for right click context menu
#include <QColorDialog>//for right click context menu


#include <QVTKOpenGLNativeWidget.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkProperty.h>
#include <vtkRendererCollection.h>

#include <QMessageBox>//forsave color config
#include <QDate>//forsave color config
#include <QFileInfo>//forsave color config



/**
 * @file mainwindow.cpp
 * @brief Implementation of the MainWindow constructor.
 */

/**
 * @brief Constructs the MainWindow object.
 *
 * This constructor initializes the main window, sets up the UI components,
 * configures the data model for the tree view, establishes signal-slot connections,
 * and prepares a simple VTK rendering scene for demonstration.
 *
 * @param parent The parent widget, typically passed as nullptr for main windows.
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);//initialize

    // Create / allocate the Model List
    this->partList = new ModelPartList("Parts List");

    // Link it to the treeview in the GUI
    ui->treeView->setModel(this->partList);

    // Manually create a model tree (quick example for starting point)
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items::wait to be modified later for items
    for (int i = 0; i < 3; i++) {
        // Create strings for both data columns
        QString name = QString("Top Level %1").arg(i);
        QString visible("true");  // ✅ 修正：原为 Qstring v i s i b l e

        // Create child item
        ModelPart* childItem = new ModelPart({name, visible});

        // Append to tree top-level
        rootItem->appendChild(childItem);

        // Add 5 sub-items
        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1, %2").arg(i).arg(j);
            QString visible("true");  // ✅ 同上

            ModelPart* childChildItem = new ModelPart({name, visible});
            childItem->appendChild(childChildItem);
        }
    }


    /**connect(ui->save,&QPushButton::clicked,this,&MainWindow::saveClicked);//connecting signal with slots
    connect(ui->open,&QPushButton::clicked,this,&MainWindow::openClicked);//connecting signal with slots
    */

    /*connect(ui->shrinkFilter,&QCheckBox::stateChanged,this,&MainWindow::shrink_FilterClicked);//connecting signal with slots
    connect(ui->clipFilter,&QCheckBox::stateChanged,this,&MainWindow::clip_FilterClicked);//connecting signal with slots note avoid deprecated*/
    connect(ui->shrinkCheckBox, &QCheckBox::checkStateChanged, this, &MainWindow::shrink_FilterClicked);
    connect(ui->clipFilter, &QCheckBox::checkStateChanged, this, &MainWindow::clip_FilterClicked);

    connect(this, &MainWindow::statusUpdateMessage,ui->statusbar, &QStatusBar::showMessage);//connecting signals with status bar
    connect(ui->treeView, &QTreeView::clicked,this, &MainWindow::handleTreeClicked);//connecting treeview items

    //connect(ui->actionOpen_File, &QAction::triggered,this, &MainWindow::on_actionOpen_File_triggered);//Openfile action, note:linked directly by ui viaMOC槽函数在此处被重复调用，引掉以避免ui及cpp中的重复调用，窗口现在仅弹出一次，成功
    //connect(ui->actionSave_File, &QAction::triggered, this, &MainWindow::saveColorConfiguration);//save file function moved to on actionSave File triggered
    //connect(ui->actionSave_File, &QAction::triggered,this, &MainWindow::on_actionSave_File_triggered);//Savefile action

    //connecting rgb vertical slider with private slots
    connect(ui->rSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);
    connect(ui->gSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);
    connect(ui->bSlider, &QSlider::valueChanged, this, &MainWindow::updateColorFromSliders);

    //adding right click
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, &MainWindow::showContextMenu);

    connect(ui->horizontalSlider_2, &QSlider::valueChanged, this, &MainWindow::updateLighting);

    ui->shrinkCheckBox->setEnabled(false);
    ui->clipFilter->setEnabled(false);
    ui->actionExitVR->setEnabled(false);//disable exitVR at normal window


    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(16);

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> actor;
    actor->SetMapper(mapper);

    mainRenderer = vtkSmartPointer<vtkRenderer>::New();
    mainRenderer->SetBackground(0.1, 0.2, 0.4);
    mainRenderer->AddActor(actor);
    ui->VTKOpenGLNativeWidget->renderWindow()->AddRenderer(mainRenderer);    //note:new version vtk changed the coding
    /** ✅ 关键：使用正确名称的 VTK widgetui->VTKOpenGLNativeWidget->GetRenderWindow()->AddRenderer(renderer);*/



}

/**
 * @brief Destructor for MainWindow.
 *
 * Releases the UI resources and cleans up memory.
 */
MainWindow::~MainWindow()
{
    delete ui;//release memory
}
/**void MainWindow::saveClicked(){
    //qDebug()<<"Button Clicked";
    emit statusUpdateMessage("Saved!", 3000);  //save file::wait to be add to the file list later
}

void MainWindow::openClicked(){
    emit statusUpdateMessage("Opened!", 3000);  //open file::wait to be add to the file list later
    //wait to be added loop for adding all the files
};//open
*/
/**
 * @brief Slot function triggered when the shrink filter checkbox is toggled.
 *
 * Enables or disables the shrink filter on the currently selected model part based on checkbox state.
 * If enabled, the shrink filter is applied; otherwise, the geometry is reset to original.
 * The rendering window is updated to reflect the change.
 *
 * @param state The checkbox state (Qt::Checked or Qt::Unchecked).
 */
void MainWindow::shrink_FilterClicked(int state)
{
    if (currentSelectedPart) {
        currentSelectedPart->setShrinkEnabled(state == Qt::Checked);

        if (state == Qt::Checked)
            currentSelectedPart->applyShrinkFilter();
        else
            currentSelectedPart->resetGeometry();  // 取消滤镜
    }

    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
}
//checkbox shrink
/**
 * @brief Slot function triggered when the clip filter checkbox is toggled.
 *
 * Enables or disables the clipping filter on the currently selected model part depending on checkbox state.
 * If enabled, the clip filter is applied; if disabled, the geometry is restored to its original state.
 * The rendering window is refreshed accordingly.
 *
 * @param state The checkbox state (Qt::Checked or Qt::Unchecked).
 */
void MainWindow::clip_FilterClicked(int state)
{
    if (currentSelectedPart) {
        currentSelectedPart->setClipEnabled(state == Qt::Checked);

        if (state == Qt::Checked)
            currentSelectedPart->applyClipFilter();
        else
            currentSelectedPart->resetGeometry();  // 取消剪切
    }

    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
}

//checkbox clip
/*
void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    // 获取点击的 ModelPart
    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    QString name = part->data(0).toString();  // 第0列是名称
    QString visible = part->data(1).toString();  // 第1列是可见性

    // 显示在状态栏
    emit statusUpdateMessage("Selected: " + name + " (Visible: " + visible + ")", 3000);

    // 此处可加入其他操作，例如高亮、渲染等
}*/
/**void MainWindow::handleTreeClicked(const QModelIndex &index)//note:remember set max and min of slider
{
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    currentSelectedPart = part;//saving the current selected part

    if (part->getActor()) {//set new model
        double* rgb = part->getActor()->GetProperty()->GetColor();
        ui->rSlider->setValue(static_cast<int>(rgb[0] * 255));
        ui->gSlider->setValue(static_cast<int>(rgb[1] * 255));
        ui->bSlider->setValue(static_cast<int>(rgb[2] * 255));

        //part->getActor()->GetProperty()->SetColor(1.0, 0.0, 0.0); // 红
    }


    emit statusUpdateMessage("Selected: " + part->data(0).toString(), 3000);
    //quoted and changed by upper
    // 重置所有 actor 为默认颜色
    //for (int i = 0; i < partList->rowCount(QModelIndex()); ++i) {
    //    ModelPart* child = static_cast<ModelPart*>(partList->index(i, 0, QModelIndex()).internalPointer());

    //    if (child && child->getActor()) {
    //        child->getActor()->GetProperty()->SetColor(1.0, 1.0, 1.0); // 白
    //    }
    //}

    // 设置当前选中 actor 为高亮颜色
    if (part->getActor()) {
        part->getActor()->GetProperty()->SetColor(1.0, 0.0, 0.0); // 红
    }
//    ui->VTKOpenGLNativeWidget->renderWindow()->Render(); // 更新
}*/
/**void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    currentSelectedPart = part;

    if (part->getActor()) {
        double* rgb = part->getActor()->GetProperty()->GetColor();

        // 👇 自动同步到 slider
        ui->rSlider->setValue(static_cast<int>(rgb[0] * 255));
        ui->gSlider->setValue(static_cast<int>(rgb[1] * 255));
        ui->bSlider->setValue(static_cast<int>(rgb[2] * 255));
    }

    emit statusUpdateMessage("Selected: " + part->data(0).toString(), 3000);
    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
}*/
/**
 * @brief Slot function triggered when a tree view item is clicked.
 *
 * Sets the clicked item as the currently selected model part. Updates the RGB sliders
 * to reflect the part's current color, synchronizes the shrink and clip checkbox states,
 * and sets the lighting slider. Also updates the status bar and triggers a re-render.
 *
 * @param index The QModelIndex corresponding to the clicked tree item.
 */
void MainWindow::handleTreeClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part || !part->getActor()) return;

    // 设置当前选中模型
    currentSelectedPart = part;

    // 读取模型颜色并更新 slider，不修改 actor 本身的颜色
    double* rgb = part->getActor()->GetProperty()->GetColor();
    ui->rSlider->blockSignals(true);
    ui->gSlider->blockSignals(true);
    ui->bSlider->blockSignals(true);
    ui->rSlider->setValue(static_cast<int>(rgb[0] * 255));
    ui->gSlider->setValue(static_cast<int>(rgb[1] * 255));
    ui->bSlider->setValue(static_cast<int>(rgb[2] * 255));
    ui->rSlider->blockSignals(false);
    ui->gSlider->blockSignals(false);
    ui->bSlider->blockSignals(false);

    // 添加以下代码，用于同步 UI 控件状态
    ui->shrinkCheckBox->setChecked(part->isShrinkEnabled());
    ui->clipFilter->setChecked(part->isClipEnabled());
    ui->horizontalSlider_2->setValue(static_cast<int>(part->getLightValue() * 100));  // 映射到 0–100 范围

    emit statusUpdateMessage("Selected: " + part->data(0).toString(), 3000);
    ui->VTKOpenGLNativeWidget->renderWindow()->Render(); // 仅刷新画面

}



// Slot function added to MainWindow.cpp for save and open
/**void MainWindow::on_actionOpen_File_triggered() {

    // Add this line of code so you can see if the action is working
    emit statusUpdateMessage(QString("Open File action triggered"), 0);
}*/
//second version for open stl and txt file
/**void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("STL Files (*.stl);;All Files (*)")
        );

    if (!fileName.isEmpty()) {
        emit statusUpdateMessage("file selected: " + fileName, 0);
    } else {
        emit statusUpdateMessage("no file yet been chosen", 0);
    }
}*/
/**void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("All Files (*)")
        );

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("No file selected.", 0);
        return;
    }

    QFileInfo selectedFile(fileName);
    QDir directory = selectedFile.absoluteDir();
    QStringList fileList = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // 清空旧模型（如需要，可扩展为多模型共存）
    delete partList;
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    QModelIndex rootIndex = QModelIndex();
    for (const QString& file : fileList) {
        QList<QVariant> columns = { file, "true" };
        partList->appendChild(rootIndex, columns);
    }

    emit statusUpdateMessage("Loaded directory: " + directory.absolutePath(), 3000);
}*/
/**
 * @brief Slot function triggered when "Open File" action is selected.
 *
 * Opens a file dialog to select a `.stl` or other supported file, scans the directory
 * for all matching files, classifies them into STL and non-STL categories, and loads
 * them into the application. STL files are asynchronously processed and rendered using VTK,
 * while other files are displayed in the model tree without rendering.
 *
 * Also initializes a progress bar to track asynchronous loading and updates the status bar
 * accordingly. Previously loaded models and view props are cleared before loading new ones.
 */
void MainWindow::on_actionOpen_File_triggered()
{
    QElapsedTimer timer;
    timer.start();

    // 选择任一 .stl 或 .其他 文件
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "",
        tr("STL Files (*.stl);;txt or csv (*.txt;csv)"));

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("No file selected.", 0);
        return;
    }

    QFileInfo selectedFile(fileName);
    QDir directory = selectedFile.absoluteDir();

    // 获取所有符合条件的文件
    QStringList filters;
    filters << "*.stl" << "*";
    directory.setNameFilters(filters);
    QStringList filteredFiles = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);

    // 按后缀分类
    QStringList stlFiles, nonStlFiles;
    for (const QString& file : filteredFiles) {
        if (file.endsWith(".stl", Qt::CaseInsensitive))
            stlFiles << file;
        else
            nonStlFiles << file;  // 所有非stl文件
    }

    // 清空旧模型数据
    delete partList;
    partList = new ModelPartList("Parts List");
    ui->treeView->setModel(partList);

    // 初始化渲染器
    mainRenderer->RemoveAllViewProps();  // 清除旧模型
    mainRenderer->SetBackground(0.1, 0.2, 0.3);


    // 初始化进度条
    filesLoaded = 0;
    totalFiles = stlFiles.size();

    if (progressBar) {
        statusBar()->removeWidget(progressBar);
        delete progressBar;
    }

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, totalFiles);
    progressBar->setValue(0);
    statusBar()->addPermanentWidget(progressBar);

    // 添加分类根项
    QModelIndex rootIndex = QModelIndex();
    QModelIndex stlRoot = partList->appendChild(rootIndex, { "1.stl", "" });
    QModelIndex nonSTLRoot = partList->appendChild(rootIndex, { "2.other", "" });

    // 加载 STL 文件并渲染
    for (const QString& file : stlFiles) {
        QString fullPath = directory.absoluteFilePath(file);
        QModelIndex index = partList->appendChild(stlRoot, { file, "true" });
        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

        (void)QtConcurrent::run([this, part, fullPath]() {
            part->loadSTL(fullPath);

            QMetaObject::invokeMethod(this, [this, part]() {
                if (part->getActor()) {
                    mainRenderer->AddActor(part->getActor());
                    mainRenderer->ResetCamera();
                    ui->VTKOpenGLNativeWidget->renderWindow()->Render();

                    filesLoaded++;
                    progressBar->setValue(filesLoaded);

                    if (filesLoaded == totalFiles) {
                        statusBar()->showMessage("All files loaded", 5000);
                        QTimer::singleShot(2000, this, [this]() {
                            statusBar()->removeWidget(progressBar);
                            delete progressBar;
                            progressBar = nullptr;
                        });
                    }
                }
            }, Qt::QueuedConnection);
        });

    }

    // 加载 other 文件到 TreeView（不参与渲染）
    for (const QString& file : nonStlFiles) {
        partList->appendChild(nonSTLRoot, { file, "" });
    }
    emit statusUpdateMessage("Loaded directory: " + directory.absolutePath(), 5000);
    qDebug() << "Total load time: " << timer.elapsed() << " ms";
}

/*
void MainWindow::on_actionSave_File_triggered() {

    // Add this line of code so you can see if the action is working
    emit statusUpdateMessage(QString("Save File action triggered"), 0);

}*/
/**
 * @brief Slot function triggered when "Save File" action is selected.
 *
 * Saves raw RGB color data of all valid STL model parts currently loaded into the tree model
 * into a CSV (or TXT) file. The data includes part name, RGB color values, and associated state flags
 * such as shrink and clip filters, along with lighting intensity.
 *
 * If no parts are loaded or no valid model is found, the function displays appropriate warnings.
 * The CSV includes a header row and recursively traverses all nodes in the model tree.
 */
void MainWindow::on_actionSave_File_triggered()
{
    if (partList->rowCount(QModelIndex()) == 0) {
        QMessageBox::warning(this, "Error", "No parts loaded.");
        return;
    }

    QModelIndex topGroup = partList->index(0, 0, QModelIndex());
    QModelIndex firstModel = partList->index(0, 0, topGroup);
    ModelPart* firstPart = findFirstValidPart(partList->getRootItem());

    if (!firstPart || firstPart->filePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "No valid STL model found.");
        return;
    }


    QFileInfo fileInfo(firstPart->filePath);
    QString folderName = fileInfo.dir().dirName();
    QString defaultFileName = folderName + "_rawRGB.csv";
    QString defaultPath = fileInfo.dir().absoluteFilePath(defaultFileName);

    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Save Raw RGB Color Data",
        defaultPath,
        "CSV Files (*.csv);;Text Files (*.txt)");

    if (savePath.isEmpty()) {
        statusBar()->showMessage("Save canceled.", 3000);
        return;
    }

    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot write to file.");
        return;
    }

    QTextStream out(&file);

    // 添加新的表头列
    out << "PartName,R,G,B,Shrink,Clip,Light\n";

    std::function<void(const QModelIndex&)> recurse;
    recurse = [&](const QModelIndex& parent) {
        int rowCount = partList->rowCount(parent);
        for (int i = 0; i < rowCount; ++i) {
            QModelIndex index = partList->index(i, 0, parent);
            ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
            if (part && part->getActor()) {
                double* rgb = part->getActor()->GetProperty()->GetColor();
                QString name = QFileInfo(part->filePath).fileName();

                out << name << "," << rgb[0] << "," << rgb[1] << "," << rgb[2]
                    << "," << part->isShrinkEnabled()
                    << "," << part->isClipEnabled()
                    << "," << part->getLightValue()
                    << "\n";
            }
            recurse(index);
        }
    };

    recurse(QModelIndex());
    file.close();


    statusBar()->showMessage("Raw RGB color data saved to " + savePath, 5000);
}
/*
void MainWindow::on_actionSave_File_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Configuration"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (fileName.isEmpty()) {
        emit statusUpdateMessage("Save canceled.", 0);
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QTextStream out(&file);

    // Traverse through the part list and write each part's color
    QModelIndex rootIndex = QModelIndex();
    for (int i = 0; i < partList->rowCount(rootIndex); ++i) {
        ModelPart* part = static_cast<ModelPart*>(partList->index(i, 0, rootIndex).internalPointer());
        if (part && part->getActor()) {
            double* color = part->getActor()->GetProperty()->GetColor();
            out << part->data(0).toString() << " "
                << color[0] << " "
                << color[1] << " "
                << color[2] << "\n";
        }
    }

    file.close();
    emit statusUpdateMessage("Configuration saved to " + fileName, 3000);
}*/


/**
 * @brief Updates the color of the currently selected model part based on RGB slider values.
 *
 * Reads values from the R, G, and B sliders, normalizes them to the [0,1] range,
 * and applies the resulting color to the actor's property. Triggers a render refresh
 * and emits a status message showing the updated color.
 */
void MainWindow::updateColorFromSliders()
{
    if (!currentSelectedPart || !currentSelectedPart->getActor()) return;

    double r = ui->rSlider->value() / 255.0;
    double g = ui->gSlider->value() / 255.0;
    double b = ui->bSlider->value() / 255.0;

    currentSelectedPart->getActor()->GetProperty()->SetColor(r, g, b);
    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
    emit statusUpdateMessage(QString("Color updated: R=%1, G=%2, B=%3")
                                 .arg(ui->rSlider->value())
                                 .arg(ui->gSlider->value())
                                 .arg(ui->bSlider->value()), 1000);
}
/**
 * @brief Recursively finds the first valid model part that has a non-empty file path.
 *
 * Traverses the model part tree in depth-first order, returning the first node
 * that corresponds to a valid STL-loaded part (i.e., has a file path).
 *
 * @param parent The root or current node to start the search from.
 * @return Pointer to the first valid ModelPart found, or nullptr if none exist.
 */
ModelPart* MainWindow::findFirstValidPart(ModelPart* parent) {
    if (!parent) return nullptr;
    if (!parent->filePath.isEmpty()) return parent;

    for (int i = 0; i < parent->childCount(); ++i) {
        ModelPart* child = parent->child(i);
        ModelPart* found = findFirstValidPart(child);
        if (found) return found;
    }
    return nullptr;
}

/**
 * @brief Slot function triggered when "Load" action is selected.
 *
 * Opens a CSV file containing previously saved color configuration data, where each row specifies
 * a model part's file name and its RGB color values. The function parses the file, searches for matching
 * model parts in the scene by filename, and applies the stored color settings.
 * If the modified part is currently selected, the RGB sliders are updated accordingly.
 *
 * At the end of the operation, the render window is refreshed and a status message is shown.
 */
void MainWindow::on_actionLoad_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load Color Configuration", "", "CSV Files (*.csv)");

    if (filePath.isEmpty()) {
        statusBar()->showMessage("Load canceled.", 3000);
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open file.");
        return;
    }

    QTextStream in(&file);
    QString header = in.readLine(); // Skip header line

    // 读取每一行并应用颜色
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(",");
        if (parts.size() != 4) continue;

        QString fileName = parts[0];
        double r = parts[1].toDouble();
        double g = parts[2].toDouble();
        double b = parts[3].toDouble();

        std::function<void(ModelPart*)> applyColor;
        applyColor = [&](ModelPart* part) {
            if (!part) return;

            if (QFileInfo(part->filePath).fileName() == fileName) {
                if (part->getActor()) {
                    part->getActor()->GetProperty()->SetColor(r, g, b);
                    if (part == currentSelectedPart) {
                        ui->rSlider->blockSignals(true);
                        ui->gSlider->blockSignals(true);
                        ui->bSlider->blockSignals(true);
                        ui->rSlider->setValue(static_cast<int>(r * 255));
                        ui->gSlider->setValue(static_cast<int>(g * 255));
                        ui->bSlider->setValue(static_cast<int>(b * 255));
                        ui->rSlider->blockSignals(false);
                        ui->gSlider->blockSignals(false);
                        ui->bSlider->blockSignals(false);
                    }
                }
            }

            for (int i = 0; i < part->childCount(); ++i) {
                applyColor(part->child(i));
            }
        };

        applyColor(partList->getRootItem());
    }

    file.close();
    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
    statusBar()->showMessage("Color configuration loaded from " + filePath, 5000);
}

/**
 * @brief Displays a custom context menu for the selected model part in the tree view.
 *
 * Provides a right-click context menu with options to toggle visibility, rename the model,
 * change its color, apply filters (shrink/clip), add child STL models, delete the selected model,
 * and reset all filters. Each action triggers the appropriate model manipulation or rendering update.
 *
 * The menu dynamically applies changes to the selected model part and updates the VTK renderer and UI state.
 *
 * @param pos The position within the tree view where the context menu was requested.
 */
void MainWindow::showContextMenu(const QPoint& pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    if (!index.isValid()) return;

    ModelPart* part = static_cast<ModelPart*>(index.internalPointer());
    if (!part) return;

    QMenu contextMenu(this);

    QAction* toggleVisibility = contextMenu.addAction("Toggle Visibility");
    QAction* renamePart = contextMenu.addAction("Rename Model");
    QAction* changeColor = contextMenu.addAction("Change Color");
    contextMenu.addSeparator();

    QMenu* filterMenu = contextMenu.addMenu("Apply Filter");
    QAction* shrinkAction = filterMenu->addAction("Shrink");
    QAction* clipAction = filterMenu->addAction("Clip");
    contextMenu.addSeparator();

    QAction* addModel = contextMenu.addAction("Add Child Model");
    QAction* removeModel = contextMenu.addAction("Delete This Model");

    QAction* resetFilters = contextMenu.addAction("Reset Filters");

    QAction* selectedAction = contextMenu.exec(ui->treeView->viewport()->mapToGlobal(pos));
    if (!selectedAction) return;

    if (selectedAction == toggleVisibility) {
        bool visible = part->visible();
        part->setVisible(!visible);
        /*if (part->getActor()) {
            part->getActor()->SetVisibility(!visible);
            ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        }*/
        part->setVisible(!visible);
        ui->VTKOpenGLNativeWidget->renderWindow()->Render();

    } else if (selectedAction == renamePart) {
        bool ok;
        QString newName = QInputDialog::getText(this, "Rename Model", "New name:", QLineEdit::Normal, part->data(0).toString(), &ok);
        if (ok && !newName.trimmed().isEmpty()) {
            part->set(0, newName);
            partList->layoutChanged();
        }
    } else if (selectedAction == changeColor) {
        QColor color = QColorDialog::getColor(Qt::white, this, "Select Color");
        if (color.isValid() && part->getActor()) {
            part->getActor()->GetProperty()->SetColor(color.redF(), color.greenF(), color.blueF());
            ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        }
    } else if (selectedAction == addModel) {
        QString filePath = QFileDialog::getOpenFileName(this, "Add STL Model", "", "STL Files (*.stl)");
        if (filePath.isEmpty()) return;

        QFileInfo info(filePath);
        QString name = info.fileName();

        // 检查在父类下是否已有同名子模型
        /**for (int i = 0; i < part->childCount(); ++i) {
            if (part->child(i)->data(0).toString() == name) {
                QMessageBox::warning(this, "Duplicate", "This file is already loaded under this part.");
                return;
            }
        }*/
        //checking through whole dic
        if (isSTLAlreadyLoaded(filePath)) {
            QMessageBox::warning(this, "Duplicate", "This STL file is already loaded in the model tree.");
            return;
        }


        emit statusUpdateMessage("Loading model: " + name, 2000);

        ModelPart* newPart = new ModelPart({ name, "true" }, part);
        newPart->loadSTL(filePath);
        newPart->filePath = filePath;

        // 继承颜色与可见性
        if (part->getActor() && newPart->getActor()) {
            double* rgb = part->getActor()->GetProperty()->GetColor();
            newPart->getActor()->GetProperty()->SetColor(rgb[0], rgb[1], rgb[2]);
            newPart->setVisible(part->visible());
            newPart->getActor()->SetVisibility(part->visible() ? 1 : 0);
        }

        part->appendChild(newPart);

        if (newPart->getActor()) {
            mainRenderer->AddActor(newPart->getActor());
            mainRenderer->ResetCamera();
            ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        }

        partList->layoutChanged();
        emit statusUpdateMessage("Model loaded: " + name, 3000);
    } else if (selectedAction == removeModel) {
        if (QMessageBox::question(this, "Confirm Delete", "Delete this model and all submodels?") == QMessageBox::Yes) {

            std::function<void(ModelPart*)> removeActorsRecursively;
            removeActorsRecursively = [&](ModelPart* part) {
                if (part->getActor()) {
                    mainRenderer->RemoveActor(part->getActor());
                }
                for (int i = 0; i < part->childCount(); ++i) {
                    removeActorsRecursively(part->child(i));
                }
            };
            removeActorsRecursively(part);

            if (!partList->removePart(index)) {
                QMessageBox::warning(this, "Error", "Failed to delete model.");
            } else {
                if (currentSelectedPart == part) currentSelectedPart = nullptr;
                ui->VTKOpenGLNativeWidget->renderWindow()->Render();
            }
        }
    } else if (selectedAction == shrinkAction) {
        part->applyShrinkFilter();
        ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        emit statusUpdateMessage("Shrink filter applied to: " + part->data(0).toString(), 2000);
    } else if (selectedAction == clipAction) {
        part->applyClipFilter();
        ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        emit statusUpdateMessage("Clip filter applied to: " + part->data(0).toString(), 2000);
    }else if (selectedAction == resetFilters) {
        part->resetGeometry();
        ui->VTKOpenGLNativeWidget->renderWindow()->Render();
        emit statusUpdateMessage("Reset filters for: " + part->data(0).toString(), 2000);
    }
}


//checking if exist
/**bool MainWindow::isSTLAlreadyLoaded(const QString& filePath) {
    QFileInfo newFileInfo(filePath);
    QString newFileName = newFileInfo.fileName();

    std::function<bool(ModelPart*)> check;
    check = [&](ModelPart* part) -> bool {
        if (!part) return false;

        QFileInfo existingInfo(part->filePath);
        if (!part->filePath.isEmpty() && existingInfo.fileName() == newFileName) {
            return true;
        }

        for (int i = 0; i < part->childCount(); ++i) {
            if (check(part->child(i))) return true;
        }
        return false;
    };

    return check(partList->getRootItem());
}*/
/**
 * @brief Checks whether an STL file with the same filename has already been loaded.
 *
 * Traverses the model part tree recursively to determine if any part's file name matches
 * the file name of the given path (case-insensitive). Only file names are compared; full paths are ignored.
 *
 * @param filePath The full path of the STL file to check.
 * @return True if a model with the same file name is already loaded; otherwise, false.
 */
bool MainWindow::isSTLAlreadyLoaded(const QString& filePath) {
    QFileInfo newFileInfo(filePath);
    QString newFileName = newFileInfo.fileName(); // 仅检查文件名，不含路径

    std::function<bool(ModelPart*)> check;
    check = [&](ModelPart* part) -> bool {
        if (!part) return false;

        QFileInfo existingInfo(part->filePath);
        if (!part->filePath.isEmpty() &&
            existingInfo.fileName().compare(newFileName, Qt::CaseInsensitive) == 0) {
            return true;
        }

        for (int i = 0; i < part->childCount(); ++i) {
            if (check(part->child(i))) return true;
        }
        return false;
    };

    return check(partList->getRootItem());
}

/**
 * @brief Updates the lighting properties of the currently selected model part.
 *
 * Maps the input slider value to a normalized intensity and applies it to the actor's material properties.
 * Adjusts ambient and diffuse lighting to maintain visual balance, and sets a fixed specular value.
 * Refreshes the VTK render window and emits a status message indicating the new ambient value.
 *
 * @param value The lighting intensity value from the slider, expected in the range [0, 100].
 */
void MainWindow::updateLighting(int value)
{
    if (!currentSelectedPart || !currentSelectedPart->getActor()) return;

    // 将滑块值映射到 [0.0, 1.0]
    double intensity = static_cast<double>(value) / 100.0;

    vtkSmartPointer<vtkActor> actor = currentSelectedPart->getActor();
    vtkSmartPointer<vtkProperty> prop = actor->GetProperty();

    // 可自由选择应用到 ambient、diffuse、specular
    prop->SetAmbient(intensity);
    prop->SetDiffuse(1.0 - intensity);
    prop->SetSpecular(0.5);  // for light spec

    ui->VTKOpenGLNativeWidget->renderWindow()->Render();
    emit statusUpdateMessage(QString("Lighting adjusted: Ambient=%1").arg(intensity), 1000);
}

/**
 * @brief Slot function triggered when the "Start" action is activated.
 *
 * Launches a new instance of the MainWindow as a VR mode window, enabling VR-related UI controls
 * such as shrink and clip filters, and the Exit VR action. The current window is closed upon launching the new instance.
 * Note: The model data transfer to the new window is not yet implemented.
 */
void MainWindow::on_actionStart_triggered()
{
    MainWindow* vrWindow = new MainWindow(/*this->partList//could be use to transfer model, yet not sure how*/);
    vrWindow->show();

    // 启用 VR Filter 控件
    vrWindow->ui->shrinkCheckBox->setEnabled(true);
    vrWindow->ui->clipFilter->setEnabled(true);
    vrWindow->ui->actionExitVR->setEnabled(true);//enable exitVR at vr window
    vrWindow->ui->actionStart->setEnabled(false);

    this->close();
}
/**
 * @brief Slot function triggered when the "Exit VR" action is selected.
 *
 * Exits VR mode by launching a standard MainWindow instance and closing the current VR window.
 * The model data is not transferred in this version but the comment indicates support for future sharing via constructor overload.
 */
void MainWindow::on_actionExitVR_triggered()
{
    // 创建普通主窗口
    MainWindow* normalWindow = new MainWindow(/*this->partList//could be use to transfer model, yet not sure how*/);//pass the model
    normalWindow->show();

    //normalWindow->ui->actionExitVR->setEnabled(false);//disable exitVR at normal window, moved to start to avoid blank period.
    // 关闭当前（VR）窗口
    this->close();
}
/*
MainWindow::MainWindow(ModelPartList* sharedPartList, QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Use existing part list
    this->partList = sharedPartList;

    // Reconnect tree view model
    ui->treeView->setModel(partList);

    // Reselect last selection if needed
    currentSelectedPart = nullptr;

    // UI init (lighting, filter state...)
}*/
/**
 * @brief Slot function triggered when the lighting slider value changes.
 *
 * Updates the internal lighting intensity value of the currently selected model part.
 * The value from the slider (assumed range 0–100) is normalized to [0.0, 1.0] and stored,
 * potentially used later for rendering or persistence.
 *
 * @param value The new slider value representing light intensity.
 */
void MainWindow::on_lightSlider_valueChanged(int value)
{
    if (currentSelectedPart) {
        currentSelectedPart->setLightValue(value / 100.0);  // 假设滑块0-100
        // 可应用 lighting 调整逻辑
    }
}
