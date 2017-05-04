#include "servermanager.h"
#include <QHashIterator>
#include <QDebug>
#include <QJsonValue>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

ServerManager *ServerManager::instance = NULL;

ServerManager::ServerManager(){
    this->isWaiting = false;
    this->started = false;
    this->conexion = new QNetworkAccessManager(this);
    connect(this->conexion,SIGNAL(finished(QNetworkReply*)),this,SLOT(finished(QNetworkReply*)));
}

void ServerManager::start(){
    if(!this->started){
        this->request();
    }
}

ServerManager *ServerManager::getInstance(){
    if(instance == NULL){
        instance = new ServerManager();
    }
    return instance;
}

void ServerManager::request(){
    if(!this->peticiones.isEmpty()){
        this->started = true;
        qDebug() << "bandera en start";
        Peticion *p = this->peticiones.first();
        QString url = p->getUrl();
        QHashIterator <QString, QString> it(*p->getData());
        QString data = "";
        while(it.hasNext()){
            it.next();
            data += it.key() + "=" + it.value();
            if(it.hasNext()){
                data += "&";
            }

        }
        if(p->getIsPost()){
            qDebug() << data;
            QByteArray postData;
            postData.append(data.toLower());
            this->conexion->post(QNetworkRequest(QUrl(url)),postData);
        }else{
            url +=data;
            this->conexion->get(QNetworkRequest(QUrl(url)));
        }
        this->isWaiting = true;
    }else{
        this->started = false;
    }
}

void ServerManager::addPeticion(Peticion *p){
    this->peticiones.append(p);
}

void ServerManager::addPeticion(QList<Peticion *> *p){
    this->peticiones.append(*p);
}

void ServerManager::finished(QNetworkReply *reply){
    QString res = reply->readAll();
    res = res.toUtf8();
    Peticion *p = this->peticiones.first();
    this->peticiones.removeFirst();
    if(reply->error() == QNetworkReply::NoError){
        p->success(res);
    }else{
        int status = 0;
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        if(statusCode.isValid()){
            status = statusCode.toInt();
        }
        p->error(status,res);
    }
    delete p;
    this->isWaiting = false;
    this->request();
}

void ServerManager::waitFor(){
    while (this->isWaiting) {
        this->delay(50);
    }
}

void ServerManager::delay(int millisecondsToWait)
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}
