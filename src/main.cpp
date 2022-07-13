#include <QApplication>
#include <QGuiApplication>
#include <QList>
#include <QScreen>
#include <QImage>
#include "home/home.h"
#include "login/login.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QMainWindow* window;

    Store store;

    if(store.get("token").toString().isEmpty())
    {
        window = new Login(store);
    }else{
        window = new Home(store);
    }

    window->show();
    
    return a.exec();
}
