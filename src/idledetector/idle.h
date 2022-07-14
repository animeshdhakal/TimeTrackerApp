#ifndef IDLE_H
#define IDLE_H

#include <QObject>
#include <QCursor>
#include <QDateTime>
#include <QTimer>


class Idle
{
public:
	static int getSystemIdleTime();
};


#endif