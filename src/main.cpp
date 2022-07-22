#include <QApplication>
#include <QGuiApplication>
#include <QList>
#include <QScreen>
#include <QImage>
#include "home/home.h"
#include "login/login.h"
#include "activity/activity.h"
#include <QTimer>
#include <QHttpMultiPart>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Store store;
    Home home(store);
    Login login(store, home);

    home.setFixedSize(home.size());
    login.setFixedSize(login.size());

    if(store.get("token").toString().isEmpty())
    { 
        login.show();
    }else{
        home.show();
    }

    qDebug() << store.get("server").toString();

    return app.exec();
}
