#include "activity.h"

#ifdef Q_OS_WIN

#include <QtCore>
#include "qt_windows.h"
#include "psapi.h"
#include "windowinfo.h"

WindowInfo Activity::getActiveWindow(){
    WindowInfo windowInfo;

    TCHAR buf[255];
    HWND foregroundWindow = GetForegroundWindow();
    DWORD* processID = new DWORD;
    GetWindowText(foregroundWindow, buf, 255);
    GetWindowThreadProcessId(foregroundWindow, processID);
    DWORD p = *processID;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_READ,
                                  FALSE, p);
    TCHAR szProcessName[MAX_PATH];

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
                                 &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }
    CloseHandle(hProcess);
    long pid = (long)p;

    windowInfo.setWindowTitle(QString::fromWCharArray(buf));
    windowInfo.setProcessName(QString::fromWCharArray(szProcessName));
    windowInfo.setPID(pid);

    return windowInfo;
}


int Activity::getSystemIdleTime(){
    LASTINPUTINFO lastInputInfo;
    lastInputInfo.cbSize = sizeof(lastInputInfo);
    GetLastInputInfo(&lastInputInfo);
    return (GetTickCount() - lastInputInfo.dwTime) / 1000;
}

#endif
