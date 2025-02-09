#include "sv_main.cpp"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainApp mainWindow;
    mainWindow.show();
    return app.exec();
}
