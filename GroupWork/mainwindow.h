#ifndef MAINWINDOW_H//avoiding redefination
#define MAINWINDOW_H

#include <QMainWindow>
#include "ModelPartList.h"
#include <QProgressBar>
#include <vtkRenderer.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @file mainwindow.h
 * @brief Declaration of the MainWindow class for managing the application's main UI and model interaction.
 *
 * This header defines the MainWindow class, responsible for GUI control, model management,
 * user interaction handling, and VTK-based 3D visualization. It declares slot functions
 * for file operations, color and lighting adjustments, filter applications, and context menus.
 *
 * Includes management of model part hierarchy (via ModelPartList), renderer setup,
 * status messaging, and progress tracking for asynchronous tasks.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT//for signal and slots

public:
    MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(ModelPartList* sharedPartList, QWidget* parent = nullptr);
    ~MainWindow();

// Slot function declaration added to MainWindow
public slots://question: what's difference between public slots and public, just slots and variables?
    //allow file filter of txt and stl
    void on_actionOpen_File_triggered();
    void on_actionSave_File_triggered();
    void on_actionLoad_triggered();


private:
    Ui::MainWindow *ui;
    //void saveClicked();//save
    //void openClicked();//open
    void saveColorConfiguration();//saving config for all model

    void handleTreeClicked(const QModelIndex &index);//Treeview slot for modifying items
    ModelPart* findFirstValidPart(ModelPart* parent);
    void shrink_FilterClicked(int state);//filter_shrink
    void clip_FilterClicked(int state);//filter_clip
    void on_lightSlider_valueChanged(int value);//lightSlider

    void showContextMenu(const QPoint& pos);//right click

    bool isSTLAlreadyLoaded(const QString& filePath);//checking stl for loading child


    //progress bar
    QProgressBar* progressBar = nullptr;
    int filesLoaded = 0;
    int totalFiles = 0;

    ModelPartList* partList;

    ModelPart* currentSelectedPart = nullptr;//for rgb adjustment in mainwindow

    vtkSmartPointer<vtkRenderer> mainRenderer;//for deleting renderer

    void updateLighting(int value);





private slots:
    void updateColorFromSliders();
    void on_actionStart_triggered();  // VR start action
    void on_actionExitVR_triggered();  // exit VR



signals:
    void statusUpdateMessage(const QString &message, int timeout);//signal for status bar
};
#endif // MAINWINDOW_H

