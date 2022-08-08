#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QString>

namespace Notification {
    void init(const char* app);
    void show(const char* title, const char* message, int timeout = 0);
}

#endif // NOTIFICATION_H