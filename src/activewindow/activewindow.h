#ifndef ACTIVE_WINDOW_H
#define ACTIVE_WINDOW_H

#include <QList>
#include "windowinfo.h"

#ifdef Q_OS_WIN32
#include "win.h"
#endif

#ifdef Q_OS_DARWIN
#include "mac.h"
#endif

#ifdef Q_OS_LINUX
#include "linux_x11.h"
#endif

class ActiveWindow
{
    public:
        static QList<WindowInfo> getActiveWindows();

};

#endif