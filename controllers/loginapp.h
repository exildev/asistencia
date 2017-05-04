#ifndef LOGINAPP_H
#define LOGINAPP_H
#include "abratractapp.h"

class LoginApp: public AbratractApp
{
    Q_OBJECT
public:
    LoginApp();
    LoginApp(QObject *context);
    ~LoginApp();
signals:
    void login_success();
public slots:
    void login(QString username, QString password);
};

#endif // LOGINAPP_H
