#include "updatechecker.h"


void UpdateChecker::checkForUpdate()
{
    QUrl url(SERVER_URL);
    url.setPath("/version");
    request.setUrl(url);
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onCheckForUpdateResponse(QNetworkReply*)));
    manager.get(request);
}


void UpdateChecker::onCheckForUpdateResponse(QNetworkReply* reply)
{
    if(reply->error() == QNetworkReply::NetworkError::NoError){
        QString version = reply->readAll();
        QString currentVersion = VERSION;

        log.debug("Current Version: " + currentVersion + " - Latest Version: " + version);

        if(version != currentVersion){
            log.info("New Version Available");
            QMessageBox::information(nullptr, "Update Available", "New Version " + version + " is available. Current Version is " + currentVersion);
        }
    }
}