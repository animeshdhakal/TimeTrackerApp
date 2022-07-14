#include "idle.h"

#ifdef Q_OS_WIN32

#include <windows.h>

int Idle::getSystemIdleTime(){
    LASTINPUTINFO lastInputInfo;
    lastInputInfo.cbSize = sizeof(lastInputInfo);
    GetLastInputInfo(&lastInputInfo);
    return (GetTickCount() - lastInputInfo.dwTime) / 1000;
}

#endif