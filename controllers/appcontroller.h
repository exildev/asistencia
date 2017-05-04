#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QDebug>
#include "validableapp.h"
#include "loginapp.h"
#include "mainwindow.h"
class AppController: public QObject
{
    Q_OBJECT
public:
    ~AppController();
    static AppController *getInstance();
    void startApp(QQmlApplicationEngine *engine);
public slots:
    void login_success();
private:
    AppController();
    static AppController *instance;
    QQmlComponent *component;
    QQmlApplicationEngine *engine;
    ValidableApp *validable_app;
    LoginApp *login_app;
    QObject *login_window;
    MainWindow *main_window;

};

#endif // APPCONTROLLER_H
