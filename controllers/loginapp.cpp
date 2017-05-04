#include "loginapp.h"
#include <QDebug>

LoginApp::LoginApp(){

}

LoginApp::LoginApp(QObject *context)
{
    this->context = context->findChild<QObject*>("funcs");
    QObject::connect(this->context,SIGNAL(login(QString,QString)),this,SLOT(login(QString,QString)));
    this->server = ServerManager::getInstance();
    this->data = NULL;
}

LoginApp::~LoginApp()
{

}

void LoginApp::login(QString username, QString password){
    qDebug() << username << password;
    QHash<QString, QString> *data = new QHash<QString, QString>();
    data->insert("username",username);
    data->insert("password", password);
    this->sendPeticion("http://104.236.33.228:9009/asistencia/session/login/", data, true);
    if(this->data->getStatus() == 200){
        qDebug() << "sesion iniciada";
        emit login_success();
    }else{
        qDebug() << this->data->getResponse();
        QMetaObject::invokeMethod(this->context,"loginError");
    }
}

