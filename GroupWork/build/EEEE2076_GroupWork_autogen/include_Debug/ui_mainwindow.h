/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "qvtkopenglnativewidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOpen_File;
    QAction *actionSave_File;
    QAction *actionLoad;
    QAction *actionStart;
    QAction *actionExitVR;
    QAction *actionreadme;
    QAction *actionPrint;
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_5;
    QWidget *widget;
    QTreeView *treeView;
    QVTKOpenGLNativeWidget *VTKOpenGLNativeWidget;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout;
    QGroupBox *materialBox;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_6;
    QWidget *widget_3;
    QVBoxLayout *verticalLayout_4;
    QSlider *rSlider;
    QLabel *RedSlider;
    QWidget *widget_4;
    QVBoxLayout *verticalLayout_5;
    QSlider *gSlider;
    QLabel *GreenSlider;
    QWidget *widget_5;
    QVBoxLayout *verticalLayout_6;
    QSlider *bSlider;
    QLabel *BlueSlider;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *shrinkCheckBox;
    QCheckBox *clipFilter;
    QGroupBox *LightingBox;
    QHBoxLayout *horizontalLayout_3;
    QSlider *horizontalSlider_2;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuVR;
    QMenu *menuhelp;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 569);
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName("actionOpen_File");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/icon/fileopen.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionOpen_File->setIcon(icon);
        actionSave_File = new QAction(MainWindow);
        actionSave_File->setObjectName("actionSave_File");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/icon/filesave.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionSave_File->setIcon(icon1);
        actionLoad = new QAction(MainWindow);
        actionLoad->setObjectName("actionLoad");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/icon/load.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionLoad->setIcon(icon2);
        actionStart = new QAction(MainWindow);
        actionStart->setObjectName("actionStart");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/icon/startVR.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionStart->setIcon(icon3);
        actionExitVR = new QAction(MainWindow);
        actionExitVR->setObjectName("actionExitVR");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/icon/stopVR.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionExitVR->setIcon(icon4);
        actionreadme = new QAction(MainWindow);
        actionreadme->setObjectName("actionreadme");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/icon/help.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionreadme->setIcon(icon5);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName("actionPrint");
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/icon/print.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        actionPrint->setIcon(icon6);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(0, 0, 371, 311));
        horizontalLayout_5 = new QHBoxLayout(layoutWidget);
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(layoutWidget);
        widget->setObjectName("widget");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Maximum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMouseTracking(false);

        horizontalLayout_5->addWidget(widget);

        treeView = new QTreeView(layoutWidget);
        treeView->setObjectName("treeView");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(treeView->sizePolicy().hasHeightForWidth());
        treeView->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(treeView);

        VTKOpenGLNativeWidget = new QVTKOpenGLNativeWidget(centralwidget);
        VTKOpenGLNativeWidget->setObjectName("VTKOpenGLNativeWidget");
        VTKOpenGLNativeWidget->setGeometry(QRect(370, 0, 421, 521));
        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(0, 310, 367, 179));
        horizontalLayout = new QHBoxLayout(layoutWidget1);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        materialBox = new QGroupBox(layoutWidget1);
        materialBox->setObjectName("materialBox");
        verticalLayout_7 = new QVBoxLayout(materialBox);
        verticalLayout_7->setObjectName("verticalLayout_7");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        widget_3 = new QWidget(materialBox);
        widget_3->setObjectName("widget_3");
        verticalLayout_4 = new QVBoxLayout(widget_3);
        verticalLayout_4->setObjectName("verticalLayout_4");
        rSlider = new QSlider(widget_3);
        rSlider->setObjectName("rSlider");
        rSlider->setMaximum(255);
        rSlider->setOrientation(Qt::Orientation::Vertical);

        verticalLayout_4->addWidget(rSlider);

        RedSlider = new QLabel(widget_3);
        RedSlider->setObjectName("RedSlider");

        verticalLayout_4->addWidget(RedSlider);


        horizontalLayout_6->addWidget(widget_3);

        widget_4 = new QWidget(materialBox);
        widget_4->setObjectName("widget_4");
        verticalLayout_5 = new QVBoxLayout(widget_4);
        verticalLayout_5->setObjectName("verticalLayout_5");
        gSlider = new QSlider(widget_4);
        gSlider->setObjectName("gSlider");
        gSlider->setMaximum(255);
        gSlider->setOrientation(Qt::Orientation::Vertical);

        verticalLayout_5->addWidget(gSlider);

        GreenSlider = new QLabel(widget_4);
        GreenSlider->setObjectName("GreenSlider");

        verticalLayout_5->addWidget(GreenSlider);


        horizontalLayout_6->addWidget(widget_4);

        widget_5 = new QWidget(materialBox);
        widget_5->setObjectName("widget_5");
        verticalLayout_6 = new QVBoxLayout(widget_5);
        verticalLayout_6->setObjectName("verticalLayout_6");
        bSlider = new QSlider(widget_5);
        bSlider->setObjectName("bSlider");
        bSlider->setMouseTracking(false);
        bSlider->setMaximum(255);
        bSlider->setOrientation(Qt::Orientation::Vertical);

        verticalLayout_6->addWidget(bSlider);

        BlueSlider = new QLabel(widget_5);
        BlueSlider->setObjectName("BlueSlider");

        verticalLayout_6->addWidget(BlueSlider);


        horizontalLayout_6->addWidget(widget_5);


        verticalLayout_7->addLayout(horizontalLayout_6);


        horizontalLayout->addWidget(materialBox);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        groupBox = new QGroupBox(layoutWidget1);
        groupBox->setObjectName("groupBox");
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName("verticalLayout");
        shrinkCheckBox = new QCheckBox(groupBox);
        shrinkCheckBox->setObjectName("shrinkCheckBox");

        verticalLayout->addWidget(shrinkCheckBox);

        clipFilter = new QCheckBox(groupBox);
        clipFilter->setObjectName("clipFilter");

        verticalLayout->addWidget(clipFilter);


        verticalLayout_2->addWidget(groupBox);

        LightingBox = new QGroupBox(layoutWidget1);
        LightingBox->setObjectName("LightingBox");
        horizontalLayout_3 = new QHBoxLayout(LightingBox);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalSlider_2 = new QSlider(LightingBox);
        horizontalSlider_2->setObjectName("horizontalSlider_2");
        horizontalSlider_2->setOrientation(Qt::Orientation::Horizontal);

        horizontalLayout_3->addWidget(horizontalSlider_2);


        verticalLayout_2->addWidget(LightingBox);


        horizontalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuVR = new QMenu(menubar);
        menuVR->setObjectName("menuVR");
        menuhelp = new QMenu(menubar);
        menuhelp->setObjectName("menuhelp");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuVR->menuAction());
        menubar->addAction(menuhelp->menuAction());
        menuFile->addAction(actionOpen_File);
        menuFile->addAction(actionSave_File);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionPrint);
        menuVR->addAction(actionStart);
        menuVR->addAction(actionExitVR);
        menuhelp->addAction(actionreadme);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen_File->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_File->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_File->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionLoad->setText(QCoreApplication::translate("MainWindow", "Load", nullptr));
#if QT_CONFIG(shortcut)
        actionLoad->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_CONFIG(shortcut)
        actionStart->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
#if QT_CONFIG(shortcut)
        actionStart->setShortcut(QCoreApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExitVR->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
#if QT_CONFIG(shortcut)
        actionExitVR->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionreadme->setText(QCoreApplication::translate("MainWindow", "readme", nullptr));
        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print", nullptr));
        materialBox->setTitle(QCoreApplication::translate("MainWindow", "Color", nullptr));
        RedSlider->setText(QCoreApplication::translate("MainWindow", "R", nullptr));
        GreenSlider->setText(QCoreApplication::translate("MainWindow", "G", nullptr));
        BlueSlider->setText(QCoreApplication::translate("MainWindow", "B", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "VR", nullptr));
        shrinkCheckBox->setText(QCoreApplication::translate("MainWindow", "Apply Shrink Filter", nullptr));
        clipFilter->setText(QCoreApplication::translate("MainWindow", "Apply Clip Filter", nullptr));
        LightingBox->setTitle(QCoreApplication::translate("MainWindow", "Lighting", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuVR->setTitle(QCoreApplication::translate("MainWindow", "VR", nullptr));
        menuhelp->setTitle(QCoreApplication::translate("MainWindow", "help", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
