#include <QApplication>
#include "mainwindow.h"
#include "controllers/appcontroller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    AppController *controller = AppController::getInstance();
    controller->startApp(&engine);
    app.setWindowIcon(QIcon(":/media/icono.png"));
    int final = app.exec();
    delete controller;
    return final;
}
