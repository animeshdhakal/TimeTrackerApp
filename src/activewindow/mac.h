#ifndef MAC_H
#define MAC_H
#include <QtCore>
#include "windowinfo.h"

class Mac {
public:
    Mac();
    QList<WindowInfo> getActiveWindows();
};

#endif // MAC_H