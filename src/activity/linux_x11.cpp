#include "activity.h"
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/scrnsaver.h>
#include <QtCore>

extern "C" {
    // SEE xprop
    #define MAXSTR 1000
    unsigned long window;
    unsigned char *prop;
    Display *display;

    unsigned char* get_string_property(const char* property_name)
    {
        Atom actual_type, filter_atom;
        int actual_format, status;
        unsigned long nitems, bytes_after;

        filter_atom = XInternAtom(display, property_name, True);
        status = XGetWindowProperty(display, window, filter_atom, 0, MAXSTR, False, AnyPropertyType,
                                    &actual_type, &actual_format, &nitems, &bytes_after, &prop);

        if (status != Success) {
            return 0;
        }

        return prop;
    }

    unsigned long get_long_property(const char* property_name)
    {
        unsigned char* prop = get_string_property(property_name);

        if (prop == 0) {
            return 0;
        }

        unsigned long long_property = prop[0] + (prop[1]<<8) + (prop[2]<<16) + (prop[3]<<24);
        return long_property;
    }

    Display* getDisplay() {
        char *display_name = NULL;
        display = XOpenDisplay(display_name);

        return display;
    }
}

WindowInfo Activity::getActiveWindow()
{
    WindowInfo windowInfo;

    Display* display = getDisplay();
    int screen = XDefaultScreen(display);
    window = RootWindow(display, screen);

    if (!window) {
        throw std::invalid_argument("Couldn't get window.");
    }

    window = get_long_property("_NET_ACTIVE_WINDOW");

    if (!window) {
        return windowInfo;
    }

    unsigned char* net_wm_name = get_string_property("_NET_WM_NAME");

    if (net_wm_name != NULL) {
        windowInfo.setWindowTitle(QString::fromUtf8(net_wm_name));
    }else{
        windowInfo.setWindowTitle(QString());
    }

    windowInfo.setPID(get_long_property("_NET_WM_PID"));

    windowInfo.setProcessName(QString::fromUtf8(get_string_property("WM_CLASS")));

    XCloseDisplay(display);

    return windowInfo;
}

int Activity::getSystemIdleTime()
{
    static XScreenSaverInfo *mit_info;
    Display *display;
    int screen;
    mit_info = XScreenSaverAllocInfo();
    if((display=XOpenDisplay(NULL)) == NULL) { return(-1); }
    screen = DefaultScreen(display);
    XScreenSaverQueryInfo(display, RootWindow(display,screen), mit_info);
    int idleTime = (mit_info->idle) / 1000;
    XFree(mit_info);
    XCloseDisplay(display); 
    return idleTime;
}

void setIcon(){
    
}