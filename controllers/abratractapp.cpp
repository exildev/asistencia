#include "abratractapp.h"

QString AbratractApp::token = "";

AbratractApp::AbratractApp()
{

}

AbratractApp::~AbratractApp()
{

}

void AbratractApp::getServerSuccess(QString response, QHash<QString, QString> *data){
    this->data = new RawData(200,response,data);
}

void AbratractApp::getServerError(int status, QString response, QHash<QString, QString> *data){
    this->data = new RawData(status,response,data);
}

void AbratractApp::sendPeticion(QString url, QHash<QString, QString> *data,bool is_post){
    /*if(is_post){
        QString token = this->getToken();
        data->insert("csrfmiddlewaretoken",token);
    }*/
    Peticion *p = new Peticion();
    p->setUrl(url);
    p->setData(data);
    p->setIsPost(is_post);
    QObject::connect(p,SIGNAL(sendSuccess(QString,QHash<QString,QString>*)),this,SLOT(getServerSuccess(QString,QHash<QString,QString>*)));
    QObject::connect(p,SIGNAL(sendError(int,QString,QHash<QString,QString>*)),this,SLOT(getServerError(int,QString,QHash<QString,QString>*)));
    this->server->addPeticion(p);
    this->server->start();
    this->server->waitFor();
}

QString AbratractApp::getToken(){
    if(AbratractApp::token == ""){
        this->sendPeticion("http://192.168.1.68:8000/asistencia/session/token/",new QHash<QString,QString>(),false);
        AbratractApp::token = this->data->getResponse();
    }
    return AbratractApp::token;
}
