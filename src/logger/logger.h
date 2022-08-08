#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

class Logger {
    public:
    Logger();
    void log(QString, QString);
    void info(QString);
    void error(QString);
    void debug(QString);
    void warning(QString);

    private:
    QString logFilePath;
};


#endif // LOGGER_H