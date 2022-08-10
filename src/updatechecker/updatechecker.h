#ifndef UPDATECHECKER_H
#define UPDATECHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMessageBox>
#include "../logger/logger.h"
#include "../definations.h"

class UpdateChecker : public QObject {
    Q_OBJECT

    public:
    void checkForUpdate();

    private slots:
    void onCheckForUpdateResponse(QNetworkReply* reply);

    private:
    QNetworkAccessManager manager;
    QNetworkRequest request;
    Logger log;
};

#endif // UPDATECHECKER_H