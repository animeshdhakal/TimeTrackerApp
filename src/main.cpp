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
#include "logger/logger.h"
#include "notification/notification.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Notification::init("TimeTracker");
    app.setApplicationName("TimeTracker");

    Store store;
    Logger log;

    log.info("----------------------------------------------------");
    
    if(store.get("token").toString().isEmpty())
    { 
        log.info("No Token Found, Opening Login Page");
        Login* login = new Login(store);
        login->setAttribute(Qt::WA_DeleteOnClose);
        login->setFixedSize(login->size());
        login->show();
    }else{
        log.info("Token Found, Opening Home Page");
        Home* home = new Home(store);
        home->setAttribute(Qt::WA_DeleteOnClose);
        home->setFixedSize(home->size());
        home->show();
    }



    return app.exec();
}
