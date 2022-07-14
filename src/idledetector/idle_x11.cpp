#include "idle.h"

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)

#include <QApplication>
#include <xcb/xcb.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/scrnsaver.h>


int Idle::getSystemIdleTime()
{
    static XScreenSaverInfo *mit_info;
    Display *display;
    int screen;
    mit_info = XScreenSaverAllocInfo();
    if((display=XOpenDisplay(NULL)) == NULL) { return(-1); }
    screen = DefaultScreen(display);
    XScreenSaverQueryInfo(display, RootWindow(display,screen), mit_info);
    XFree(mit_info);
    XCloseDisplay(display); 
    return (mit_info->idle) / 1000;
}

#endif