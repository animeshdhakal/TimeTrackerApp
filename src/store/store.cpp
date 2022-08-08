#include "store.h"


QString Store::getStoreFilePath(){
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/store.json";
}

Store::Store()
{   
    QFile file(getStoreFilePath());

    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    if(!dir.exists())
    {
        dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    }
    
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        this->store = doc.object();
    }

    file.close();
}


void Store::flush(){
    QFile file(getStoreFilePath());

    if(file.open(QIODevice::WriteOnly))
    {
        QJsonDocument doc(this->store);
        file.write(doc.toJson());
    }

    file.close();
}


void Store::set(QString key, const QJsonValue& value)
{
    this->store.insert(key, value);
    this->flush();
}


void Store::clear(QString key)
{
    this->store.remove(key);
    this->flush();
}


QJsonValue Store::get(QString key)
{
    return this->store.value(key);
}


