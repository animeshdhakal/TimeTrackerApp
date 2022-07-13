#include "store.h"


Store::Store()
{   
    QFile file("store.json");
    
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        this->store = doc.object();
    }

    file.close();
}


void Store::flush(){
    QFile file("store.json");

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


