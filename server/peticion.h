#ifndef PETICION_H
#define PETICION_H
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QHash>
#include <QList>

class Peticion: public QObject
{
    Q_OBJECT
private:
    QString url;
    QHash<QString,QString> *data;
    bool isPost;
public:
    Peticion();
    QString getUrl();
    QHash<QString,QString> *getData();
    void setData(QHash<QString,QString> *data);
    void addData(QString name, QString value);
    void success(QString response);
    void error(int status, QString response);
    void setUrl(QString url);
    void setIsPost(bool isPost);
    bool getIsPost();
signals:
    void sendSuccess(QString response, QHash<QString, QString> *data);
    void sendError(int status, QString response,QHash<QString,QString> *data);
    void noSession();
};

#endif // PETICION_H
