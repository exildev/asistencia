#ifndef RAWDATA_H
#define RAWDATA_H
#include <QString>
#include <QHash>

class RawData
{
private:
    int status;
    QString response;
    QHash<QString,QString> *data;
public:
    RawData();
    RawData(int status, QString response, QHash<QString,QString> *data);
    ~RawData();

    void setStatus(int status);
    int getStatus();

    QString getResponse();
    void setResponse(QString response);

    QHash<QString,QString> *getData();
    void setData(QHash<QString,QString> *data);
};

#endif // RAWDATA_H
