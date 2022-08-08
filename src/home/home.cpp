#include "home.h"
#include "ui_home.h"

Home::Home(Store &store, QWidget *parent) : QMainWindow(parent),
                                            ui(new Ui::Home), store(store), secondsTime(QTime(0, 0)), activity(0), screenshotInterval(0), trayIcon(nullptr)
{
    ui->setupUi(this);
    createSystemTray();
}

void Home::show()
{
    QNetworkRequest request;

    QObject::connect(&secondsTimer, &QTimer::timeout, this, [&]()
                     {
        secondsTime = secondsTime.addSecs(1);

        ui->timer_label->setText(secondsTime.toString("hh:mm:ss"));

        WindowInfo windowInfo = Activity::getActiveWindow();

        bool found = false;

        for(int i = 0; i < apps.size(); i++)
        {
            QJsonObject app = apps[i].toObject();

            if(app.value("name").toString() == windowInfo.getProcessName())
            {
                if(Activity::getSystemIdleTime() == 0){
                    app.insert("activity", app.value("activity").toInt() + 1);
                }
                app.insert("time", app.value("time").toInt() + 1);
                found = true;
                apps[i] = app;
            }
        }

        if(!found)
        {
            QJsonObject app;
            app.insert("name", windowInfo.getProcessName());
            app.insert("activity", Activity::getSystemIdleTime() == 0 ? 1 : 0);
            app.insert("time", 1);
            apps.append(app);
        }

        if(Activity::getSystemIdleTime() == 0){
            activity++;
        } });
    QObject::connect(&screenshotTimer, &QTimer::timeout, this, &Home::onScreenshotTimeout);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("Authorization", "Bearer " + store.get("token").toString().toUtf8());

    request.setUrl(QUrl(store.get("server").toString() + "/api/project/all"));

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Home::onAllProjectsResponse);
    QObject::connect(ui->projects, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelectProject(int)));

    manager.post(request, QByteArray());

    QMainWindow::show();
}

Home::~Home()
{
    delete ui;
    delete trayIcon;
}

void Home::hideEvent(QHideEvent* event)
{
    this->hide();
}

void Home::createSystemTray(){
    trayIcon = new QSystemTrayIcon(this);

    QMenu* trayMenu = new QMenu(this);

    trayIcon->setParent(trayMenu);
    
    trayIcon->setIcon(QIcon("images/icon.png"));

    QAction *exitAction = new QAction(tr("Exit"), trayIcon);

    QObject::connect(exitAction, &QAction::triggered, this, &Home::close);

    QObject::connect(trayIcon, &QSystemTrayIcon::activated, this, [&](){
        if(isVisible())
        {
            this->hide();
        }
        else
        {
            this->show();
            this->activateWindow();
        }
    });

    trayIcon->setParent(trayMenu);

    trayMenu->addAction(exitAction);

    trayIcon->setContextMenu(trayMenu);

    trayIcon->show();
}

void Home::on_timer_button_clicked()
{
    if (secondsTimer.isActive())
    {
        log.info("Timer stopped");

        finishTimer();

        secondsTimer.stop();

        screenshotTimer.stop();

        ui->projects->setEnabled(true);

        ui->timer_button->setText("Start");
    }
    else
    {
        log.info("Timer started");

        startTimer();

        secondsTimer.start(1000);

        screenshotTimer.start(screenshotInterval * 1000 * 60);

        ui->projects->setDisabled(true);

        ui->timer_button->setText("Stop");
    }
}

void Home::on_signout_button_clicked(){
    store.clear("token");
    this->close();
    Login* login = new Login(store);
    login->setFixedSize(login->size());
    login->setAttribute(Qt::WA_DeleteOnClose);
    login->show();
}

void Home::startTimer()
{
    QNetworkRequest request;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("Authorization", "Bearer " + store.get("token").toString().toUtf8());

    request.setUrl(QUrl(store.get("server").toString() + "/api/activity/start"));

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Home::onStartFinishTimerResponse);

    QJsonObject json;
    json.insert("project_id", ui->projects->currentData().toInt());
    QJsonDocument doc(json);

    manager.post(request, doc.toJson(QJsonDocument::Compact));
}


void Home::finishTimer()
{
    QNetworkRequest request;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("Authorization", "Bearer " + store.get("token").toString().toUtf8());

    request.setUrl(QUrl(store.get("server").toString() + "/api/activity/finish"));

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Home::onStartFinishTimerResponse);

    QJsonObject json;

    json.insert("project_id", ui->projects->currentData().toInt());
    json.insert("activity", floor(((float)activity / (float)QTime(0, 0).secsTo(secondsTime)) * 100.0f));
    json.insert("apps", apps);

    apps = QJsonArray();

    QJsonDocument doc(json);

    manager.post(request, doc.toJson(QJsonDocument::Compact));
}

void Home::closeEvent(QCloseEvent *event)
{
    if (secondsTimer.isActive() && !isFinish)
    {
        event->ignore();
        this->isFinish = true;
        finishTimer();
    }
    else
    {
        event->accept();
    }
}

void Home::onAllProjectsResponse(QNetworkReply *reply)
{
    QVariant qStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = qStatusCode.toInt();

    QString json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());

    if (statusCode == 200)
    {
        QJsonArray jsonArray = jsonDocument.array();
        QJsonObject jsonObject;
        for (int i = 0; i < jsonArray.size(); i++)
        {
            jsonObject = jsonArray.at(i).toObject();
            ui->projects->addItem(jsonObject["name"].toString(), jsonObject["id"].toInt());
        }
    }
    else
    {
        QMessageBox::critical(this, "Warning", jsonDocument.object()["message"].toString());
    }

    QObject::disconnect(&manager, &QNetworkAccessManager::finished, this, &Home::onAllProjectsResponse);
}

void Home::onStartFinishTimerResponse(QNetworkReply *reply)
{
    QVariant qStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = qStatusCode.toInt();
    QString json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    if (statusCode == 200)
    {
        Notification::show("TimeTracker", jsonDocument.object()["message"].toString().toStdString().c_str());
    }
    else
    {
        QMessageBox::critical(this, "Warning", jsonDocument.object()["message"].toString());
    }

    QObject::disconnect(&manager, &QNetworkAccessManager::finished, this, &Home::onStartFinishTimerResponse);

    if (isFinish)
    {
        QApplication::quit();
    }
}

void Home::onGetProjectResponse(QNetworkReply *reply)
{
    QString json = reply->readAll();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    screenshotInterval = jsonObject["screenshot_interval"].toInt();

    qint64 lastTimer = jsonObject["last_timer"].toInteger();

    if (lastTimer != 0)
    {
        int time = (QDateTime::currentMSecsSinceEpoch() - lastTimer) / 1000;

        secondsTime = QTime(0, 0).addSecs(time);

        ui->timer_label->setText(secondsTime.toString("hh:mm:ss"));

        secondsTimer.start(1000);

        screenshotTimer.start(screenshotInterval * 1000 * 60);

        ui->timer_button->setText("Stop");
    }

    log.info("Screenshot interval: " + QString::number(screenshotInterval));
    log.info("Last timer: " + QString::number(lastTimer));

    QObject::disconnect(&manager, &QNetworkAccessManager::finished, this, &Home::onGetProjectResponse);

    ui->timer_button->setEnabled(true);
}

void Home::onSelectProject(int index)
{
    log.info("Project selected: " + ui->projects->currentData().toString());

    QNetworkRequest request;

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    request.setRawHeader("Authorization", "Bearer " + store.get("token").toString().toUtf8());

    ui->timer_button->setDisabled(true);

    QJsonObject json;

    json.insert("project_id", ui->projects->currentData().toInt());

    QJsonDocument doc(json);

    request.setUrl(QUrl(store.get("server").toString() + "/api/project/get"));

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Home::onGetProjectResponse);

    manager.post(request, doc.toJson(QJsonDocument::Compact));
}

void Home::onScreenshotUploadResponse(QNetworkReply *reply)
{
    QString json = reply->readAll();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());

    QJsonObject jsonObject = jsonDocument.object();

    Notification::show("TimeTracker", "Screenshot Uploaded");

    QObject::disconnect(&manager, &QNetworkAccessManager::finished, this, &Home::onScreenshotUploadResponse);
}

void Home::onScreenshotTimeout()
{   
    log.info("Screenshot timeout");

    QNetworkRequest request;

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart projectIDPart;
    projectIDPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    projectIDPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"project_id\""));
    projectIDPart.setBody(QString::number(ui->projects->currentData().toInt()).toUtf8());

    QHttpPart activityPart;
    activityPart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    activityPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"activity\""));
    activityPart.setBody(QString::number(floor(((float)activity / (float)QTime(0, 0).secsTo(secondsTime)) * 100.0f)).toUtf8());

    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap pixmap = screen->grabWindow(0);

    QBuffer *buffer = new QBuffer();
    buffer->open(QIODevice::WriteOnly);
    pixmap.save(buffer, "JPG", 50);

    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"image\"; filename=\"screenshot.jpg\""));
    imagePart.setBody(buffer->data());

    multiPart->append(imagePart);
    multiPart->append(projectIDPart);
    multiPart->append(activityPart);

    request.setRawHeader("Authorization", "Bearer " + store.get("token").toString().toUtf8());

    request.setUrl(QUrl(store.get("server").toString() + "/api/upload"));

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Home::onScreenshotUploadResponse);

    buffer->setParent(multiPart);

    QNetworkReply *reply = manager.post(request, multiPart);

    multiPart->setParent(reply);
}