#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace Notification {
    #ifdef Q_OS_LINUX
    void init(const char* app);
    void show(const char* title, const char* message, int timeout = 0);
    #else
    void init(QString app);
    void show(QString title, QString message, int timeout = 0);
    #endif
}

#endif // NOTIFICATION_H
