#include "notification.h"
#include <string.h>
#undef signals
#include <libnotify/notify.h>


namespace Notification {
    void init(const char* app) {
        notify_init(app);
    }
    
    void show(const char* title, const char* message, int timeout) {
        NotifyNotification* notification = notify_notification_new(title, message, NULL);
        notify_notification_set_timeout(notification, timeout * 1000);
        notify_notification_show(notification, NULL);
        g_object_unref(notification);
    }
}