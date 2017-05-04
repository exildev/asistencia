#ifndef SERVERMANAGER_H
#define SERVERMANAGER_H
#include <QHash>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QtSql/QtSql>
#include "peticion.h"


class ServerManager: public QObject
{
    Q_OBJECT
private:
    bool isWaiting;
    bool started;
    QSqlDatabase *db;
    QNetworkAccessManager *conexion;
    QList<Peticion *> peticiones;
    static ServerManager *instance;
    ServerManager();
    void request();
private slots:
    void finished(QNetworkReply *reply);
public:
    static ServerManager *getInstance();
    void addPeticion(Peticion *p);
    void addPeticion(QList<Peticion*> *p);
    void start();
    void waitFor();
    void delay(int millisecondsToWait);
};

#endif // SERVERMANAGER_H
