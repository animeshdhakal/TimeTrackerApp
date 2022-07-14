#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../home/home.h"
#include "../store/store.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(Store&, Home&, QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_login_clicked();
    void onLoginResponse(QNetworkReply* reply);

private:
    Ui::Login *ui;
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    Store& store;
    Home& home;
};
#endif // LOGIN_H
