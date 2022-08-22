#include "activity.h"
#include <QtCore>
#include <windows.h>
#pragma comment(lib, "user32.lib")

WindowInfo Activity::getActiveWindow(){
    WindowInfo windowInfo;

    HWND hwnd = GetForegroundWindow();
    if (hwnd) {
        DWORD maxPath = MAX_PATH;
        wchar_t app_path[MAX_PATH];
        DWORD pid;
        GetWindowThreadProcessId(hwnd, &pid);
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
        if (hProcess) {
            QueryFullProcessImageName(hProcess, 0, app_path, &maxPath);
            CloseHandle(hProcess);
            QString appPath = QString::fromWCharArray(app_path).trimmed();
            QFileInfo appFileInfo(appPath);
            QString fileName = appFileInfo.baseName();
            windowInfo.setProcessName(fileName);
        }

        wchar_t wnd_title[256];
        GetWindowText(hwnd, wnd_title, sizeof(wnd_title));
        windowInfo.setWindowTitle(QString::fromWCharArray(wnd_title));
        windowInfo.setPID(pid);
    }

    return windowInfo;
}


int Activity::getSystemIdleTime(){
    LASTINPUTINFO lastInputInfo;
    lastInputInfo.cbSize = sizeof(lastInputInfo);
    GetLastInputInfo(&lastInputInfo);
    return (GetTickCount() - lastInputInfo.dwTime) / 1000;
}