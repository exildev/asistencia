#ifndef ABRATRACTAPP_H
#define ABRATRACTAPP_H
#include <QObject>
#include "server/servermanager.h"
#include "rawdata.h"

class AbratractApp: public QObject
{
    Q_OBJECT
protected:
    QObject *context;
    ServerManager *server;
    RawData *data;
    void sendPeticion(QString url, QHash<QString, QString> *data, bool is_post);
    QString getToken();
    static QString token;
public:
    AbratractApp();
    ~AbratractApp();
public slots:
    void getServerSuccess(QString response, QHash<QString, QString> *data);
    void getServerError(int status, QString response,QHash<QString,QString> *data);
};

#endif // ABRATRACTAPP_H
