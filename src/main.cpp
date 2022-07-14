#include <QApplication>
#include <QGuiApplication>
#include <QList>
#include <QScreen>
#include <QImage>
#include "home/home.h"
#include "login/login.h"
#include "activewindow/activewindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Store store;
    Home home(store);
    Login login(store, home);


    home.setFixedSize(home.size());
    login.setFixedSize(login.size());

    QList<WindowInfo> windows = ActiveWindow::getActiveWindows();

    for(WindowInfo window : windows)
    {
        qDebug() << window.getProcessName() << window.getWindowTitle() << window.getPID();
    }

    if(store.get("token").toString().isEmpty())
    { 
        login.show();
    }else{
        home.show();
    }
    
    return app.exec();
}
