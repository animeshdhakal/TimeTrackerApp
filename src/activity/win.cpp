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
            if(fileName.isEmpty()){
                fileName = "Unknown App";
            }
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

void Activity::setIcon(){
    WNDCLASSEX wcex = {}; 
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE("images/icon.ico"));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "app.timetracker";
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
}