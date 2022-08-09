#ifndef ACTIVE_WINDOW_H
#define ACTIVE_WINDOW_H

#include "windowinfo.h"


namespace Activity {
    WindowInfo getActiveWindow();
    int getSystemIdleTime();
}
#endif