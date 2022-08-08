#ifndef STORE_H
#define STORE_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QFile>
#include <QStandardPaths>
#include <QDir>

class Store {
    public:
        Store();
        void set(QString, const QJsonValue&);
        QJsonValue get(QString);
        void clear(QString);
        void flush();
        QString getStoreFilePath();

    private:
    QJsonObject store;
};


#endif