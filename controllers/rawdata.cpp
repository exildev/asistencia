#include "rawdata.h"

RawData::RawData()
{

}

RawData::RawData(int status, QString response, QHash<QString, QString> *data){
    this->status = status;
    this->response = response;
    this->data = data;
}


RawData::~RawData()
{

}

int RawData::getStatus(){
    return this->status;
}

void RawData::setStatus(int status){
    this->status = status;
}

QString RawData::getResponse(){
    return this->response;
}

void RawData::setResponse(QString response){
    this->response = response;
}

QHash<QString,QString> *RawData::getData(){
    return this->data;
}

void RawData::setData(QHash<QString, QString> *data){
    this->data = data;
}
