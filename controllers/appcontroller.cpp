#include "appcontroller.h"
#include <QWidget>
AppController *AppController::instance = NULL;

AppController::AppController()
{
    this->login_window = NULL;
    this->main_window = NULL;
    this->login_app = NULL;
    this->validable_app = NULL;
}

AppController::~AppController()
{
    if(this->login_window != NULL){
        delete this->login_window;
    }
    if(this->main_window != NULL){
        delete this->main_window;
    }
    if(this->login_app != NULL){
        delete this->login_app;
    }
    if(this->validable_app != NULL){
        delete this->validable_app;
    }
}

AppController *AppController::getInstance(){
    if(instance == NULL){
        instance = new AppController();
    }
    return instance;
}

void AppController::startApp(QQmlApplicationEngine *engine){
    this->engine = engine;
    this->component = new QQmlComponent(engine, QUrl(QStringLiteral("qrc:/login.qml")));
    this->login_window = component->create();
    this->login_app = new LoginApp(this->login_window) ;
    QObject::connect(this->login_app,SIGNAL(login_success()),this,SLOT(login_success()));
}

void AppController::login_success(){
    this->main_window = new MainWindow();
    this->main_window->show();
    this->login_window->setProperty("visible", false);
    this->validable_app = new ValidableApp(this->main_window);
    this->validable_app->getValidables();
}
