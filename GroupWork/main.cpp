#include <QApplication>
#include "mainwindow.h"

/**
 * @brief Entry point of the Qt application.
 *
 * Initializes the QApplication, creates and displays the main window,
 * and starts the Qt event loop. A debug message is printed to the console
 * to confirm successful creation of the application instance.
 *
 * @param argc Argument count from the command line.
 * @param argv Argument vector from the command line.
 * @return The result code from the Qt event loop execution.
 */
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::cout<<"Created"<<std::endl;//added for checking the error message
    MainWindow w;
    w.show();
    return a.exec();
}
