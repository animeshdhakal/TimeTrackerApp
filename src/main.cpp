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
#include "updatechecker/updatechecker.h"
#include "guard/RunGuard.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    RunGuard guard("TimeTrackerApp");

    if(!guard.tryToRun()){
        QMessageBox::warning(0, "TimeTracker", "TimeTracker is already running!");
        return 0;
    }

    #ifdef Q_OS_LINUX
    app.setWindowIcon(QIcon("images/icon.png"));
    #endif  

    Notification::init("TimeTracker");
    app.setApplicationName("TimeTracker");

    UpdateChecker updateChecker;
    updateChecker.checkForUpdate();

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
