#include "validableapp.h"
#include <QDebug>
#include <QQmlProperty>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include "qdpwin/qdpwin.h"
#include <QQuickItem>

int i = 0;
ValidableApp::ValidableApp(MainWindow *window)
{
    this->context = window->getView()->rootObject()->findChild<QObject*>("funcs");
    QObject::connect(this->context,SIGNAL(getValidables()),this,SLOT(getValidables()));
    QObject::connect(this->context,SIGNAL(saveValidable(int,bool)),this,SLOT(saveValidable(int,bool)));
    QObject::connect(this->context,SIGNAL(createTemplate(int)),this,SLOT(createTemplate(int)));
    QObject::connect(this->context,SIGNAL(saveTemplate(int)),this,SLOT(saveTemplate(int)));
    QObject::connect(this->context,SIGNAL(resetTemplate()),this,SLOT(resetTemplate()));
    this->server = ServerManager::getInstance();
    this->data = NULL;
    this->qdp = QDPWin::getInstance();
    this->qdp->init();
    this->qdp->configure(window, this);
}

ValidableApp::ValidableApp(){
    this->server = ServerManager::getInstance();
    this->data = NULL;
}

ValidableApp::~ValidableApp()
{
    delete this->qdp;
}

void ValidableApp::getValidables(){
    this->sendPeticion("http://104.236.33.228:9009/asistencia/validables.json", new QHash<QString,QString>,false);
    if(this->data->getStatus() == 200){
        QJsonObject json = QJsonDocument::fromJson(this->data->getResponse().toUtf8()).object();
        QVariant returnedValue;
        QVariant list= json.toVariantMap();
        QMetaObject::invokeMethod(this->context,"renderValidables",
                                  Q_RETURN_ARG(QVariant,returnedValue),
                                  Q_ARG(QVariant,list));
    }
}

void ValidableApp::saveValidable(int id, bool flag){
    if(!flag){
        bool save = this->sendValidable(id,flag);
        QVariant returnedValue;
        QVariant s = save;
        QMetaObject::invokeMethod(this->context,"savedValidable",
                                  Q_RETURN_ARG(QVariant,returnedValue),
                                  Q_ARG(QVariant,s));
    }else if(this->validTemplate(id)){
        bool save = this->sendValidable(id,flag);
        QVariant returnedValue;
        QVariant s = save;
        QMetaObject::invokeMethod(this->context,"savedValidable",
                                  Q_RETURN_ARG(QVariant,returnedValue),
                                  Q_ARG(QVariant,s));
    }else{
        QVariant returnedValue;
        QVariant s = id;
        QMetaObject::invokeMethod(this->context,"noTemplate",
                                  Q_RETURN_ARG(QVariant,returnedValue),
                                  Q_ARG(QVariant,s));
    }
}

void ValidableApp::createTemplate(int id){
    this->qdp->start_enrollment();
}

void ValidableApp::saveTemplate(int id){
    QString templ = "[";
    for(DWORD i = 0; i < template_data->cbData; i++){
        if(i!=0){
            templ += ",";
        }
        templ += QString::number((int)template_data->pbData[i]);
    }
    templ += "]";
    QHash<QString, QString> *data = new QHash<QString, QString>();
    data->insert("validable",QString::number(id));
    data->insert("template",templ);
    this->sendPeticion("http://104.236.33.228:9009/asistencia/template/save/",data,true);
    QMetaObject::invokeMethod(this->context,"successTemplate");
}

void ValidableApp::resetTemplate(){
    this->qdp->finish_all();
    this->qdp->start_enrollment();
    QMetaObject::invokeMethod(this->context,"activarHuella");
}

void ValidableApp::onPush(int num){
    QVariant returnedValue;
    QVariant s = (4 - num);
    if(num < 4 && num > 0){
        QMetaObject::invokeMethod(this->context,"updateHuella",
                                  Q_RETURN_ARG(QVariant,returnedValue),
                                  Q_ARG(QVariant,s));
    }
}

void ValidableApp::onComplete(DATA_BLOB *template_data){
    this->template_data = template_data;
    QMetaObject::invokeMethod(this->context,"finishTemplate");
}

bool ValidableApp::sendValidable(int id, bool activo){
    QHash<QString, QString> *data = new QHash<QString, QString>();
    data->insert("id",QString::number(id));
    if(activo){
        data->insert("activo","on");
    }
    this->sendPeticion("http://104.236.33.228:9009/asistencia/validables/save/", data, true);
    if(this->data->getStatus() == 200){
        return true;
    }else{
        return false;
    }
}

bool ValidableApp::validTemplate(int validable){
    QHash<QString, QString> *data = new QHash<QString, QString>();
    data->insert("validable",QString::number(validable));
    this->sendPeticion("http://104.236.33.228:9009/asistencia/template/valid/", data, true);
    if(this->data->getStatus() == 200){
        return true;
    }else{
        return false;
    }
}

