#include "activity.h"

#ifdef Q_OS_WIN32

#include <windows.h>
#include <stringapiset.h>
#include <codecvt>
#include <string>
#include <locale>


WindowInfo Activity::getActiveWindow(){
    WindowInfo windowInfo;
    WCHAR window_title[256];
	HWND foreground_window = GetForegroundWindow();
	GetWindowTextW(foreground_window, window_title, 256);

	std:setlocale(LC_ALL, "en_US.UTF-8");

	DWORD pid;
	GetWindowThreadProcessId(foreground_window, &pid);
	// Process
	TCHAR process_filename[MAX_PATH];
	DWORD charsCarried = MAX_PATH;

	HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_QUERY_INFORMATION, false, pid);

	QueryFullProcessImageNameA(hProc, 0, process_filename, &charsCarried);

    QString fullpath = QString::fromWCharArray(process_filename);

	const qsizetype pos = fullpath.lastIndexOf("\\/");

	if (pos != -1){
		fullpath.erase(0, pos + 1);
	}

    

    windowInfo.setWindowsTitle(QString::fromWCharArray(windows_title));
    windowInfo.setProcessName(QString::fromStdString(fullpath));
    windowInfo.setPID(pid);


    return windowInfo;
}


int Activity::getSystemIdeTime(){
    LASTINPUTINFO lastInputInfo;
    lastInputInfo.cbSize = sizeof(lastInputInfo);
    GetLastInputInfo(&lastInputInfo);
    return (GetTickCount() - lastInputInfo.dwTime) / 1000;
}

#endif