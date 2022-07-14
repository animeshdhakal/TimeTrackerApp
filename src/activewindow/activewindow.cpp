#include "activewindow.h"

QList<WindowInfo> ActiveWindow::getActiveWindows()
{
    #ifdef Q_OS_WIN32
        win w;
        return w.getActiveWindows();
    #endif

    #ifdef Q_OS_DARWIN
        Mac m;
        return m.getActiveWindows();
    #endif

    #ifdef Q_OS_LINUX
        linux_x11 l;
        return l.getActiveWindows();
    #endif
}