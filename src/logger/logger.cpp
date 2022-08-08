#include "logger.h"

Logger::Logger(){
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    if(!dir.exists())
    {
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }

    this->logFilePath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/timetracker.log";
}


void Logger::log(QString level, QString message){
    QFile file(this->logFilePath);

    if(file.open(QIODevice::Append))
    {
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString log = time + " " + level + " - " + message + "\n";
        file.write(log.toUtf8());
    }

    file.close();
}

void Logger::info(QString message){
    this->log("INFO", message);
}

void Logger::error(QString message){
    this->log("ERROR", message);
}

void Logger::debug(QString message){
    this->log("DEBUG", message);
}

void Logger::warning(QString message){
    this->log("WARNING", message);
}