#include "MainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;

    // If the app was launched through file association, exec the cinescenie
    if (argc == 2) {
        w.loadCinescenie(argv[1]);
    }

    // Display and execute the app
    w.show();
    return a.exec();
}
