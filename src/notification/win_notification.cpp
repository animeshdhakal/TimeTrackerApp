#include "notification.h"
#include "wintoastlib.h"
#include "../logger/logger.h"
#include <QString>

using namespace WinToastLib;

class NotificationHandler : public IWinToastHandler {
public:
    void toastActivated() const {
    }

    void toastActivated(int actionIndex) const {
    }

    void toastDismissed(WinToastDismissalReason state) const {
    }

    void toastFailed() const {
    }
};

namespace Notification {
    void init(QString app) {
        WinToast::instance()->setAppName(app.toStdWString());
        WinToast::instance()->setAppUserModelId(app.toStdWString());
        if (!WinToast::instance()->initialize()) {
            Logger log;
            log.error("Unable to Initialize WinToast");
        }
    }
    
    void show(QString title, QString message, int timeout) {
        WinToastTemplate tmpl(WinToastTemplate::Text02);
        tmpl.setTextField(title.toStdWString(), WinToastTemplate::FirstLine);
        tmpl.setTextField(message.toStdWString(), WinToastTemplate::SecondLine);
        if (WinToast::instance()->showToast(tmpl, new NotificationHandler()) < 0) {
            Logger log;
            log.error("Unable to show toast");
        }
    }
}
