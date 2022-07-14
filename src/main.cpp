#include <QApplication>
#include <QGuiApplication>
#include <QList>
#include <QScreen>
#include <QImage>
#include "home/home.h"
#include "login/login.h"
#include "activewindow/activewindow.h"
#include "idledetector/idle.h"
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Store store;
    Home home(store);
    Login login(store, home);


    QTimer timer;

    QObject::connect(&timer, &QTimer::timeout, [&]() {
        int secondsIdle = Idle::getSystemIdleTime();
        qDebug() << "secondsIdle: " << secondsIdle;
    });

    timer.start(1000);

    home.setFixedSize(home.size());
    login.setFixedSize(login.size());

    

    if(store.get("token").toString().isEmpty())
    { 
        login.show();
    }else{
        home.show();
    }
    
    return app.exec();
}
