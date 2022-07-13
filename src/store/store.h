#ifndef STORE_H
#define STORE_H

#include <QJsonObject>
#include <QJsonValue>
#include <QJsonDocument>
#include <QFile>

class Store {
    public:
        Store();
        void set(QString, const QJsonValue&);
        QJsonValue get(QString);
        void clear(QString);
        void flush();

    private:
    QJsonObject store;
};


#endif