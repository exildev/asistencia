#include "peticion.h"
#include <QDebug>

Peticion::Peticion()
{
    this->data = new QHash<QString, QString>();
}

QHash<QString,QString> *Peticion::getData()
{
    return this->data;
}

void Peticion::setData(QHash<QString, QString> *data){
    this->data = data;
}

void Peticion::addData(QString name, QString value){
    this->data->insert(name,value);
}

void Peticion::error(int status, QString response)
{
    emit this->sendError(status, response, this->data);
}

void Peticion::success(QString response)
{
    emit this->sendSuccess(response,this->data);
}

void Peticion::setUrl(QString url)
{
    this->url = url;
}

QString Peticion::getUrl()
{
    return this->url;
}

void Peticion::setIsPost(bool isPost){
    this->isPost = isPost;
}

bool Peticion::getIsPost(){
    return this->isPost;
}
