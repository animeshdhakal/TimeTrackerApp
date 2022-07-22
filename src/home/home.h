#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMessageBox>
#include <QMap>
#include <QCloseEvent>
#include <QListWidget>
#include <QComboBox>
#include <QList>
#include <QScreen>
#include <QHttpMultiPart>
#include "../store/store.h"
#include "../activity/activity.h"

namespace Ui {
class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

public:
    explicit Home(Store&, QWidget *parent = nullptr);
    ~Home();
    void show();

private:
    void startTimer();
    void finishTimer();
    void closeEvent(QCloseEvent *event);

private slots:
    void on_timer_button_clicked();
    void onSelectProject(int index);

    void onScreenshotTimeout();

    // Http Slots
    void onAllProjectsResponse(QNetworkReply* reply);
    void onGetProjectResponse(QNetworkReply* reply);
    void onStartFinishTimerResponse(QNetworkReply* reply);
    void onScreenshotUploadResponse(QNetworkReply* reply);

private:
    bool isFinish = false;
    int activity;
    int screenshotInterval;
    Ui::Home *ui;
    Store& store;
    QTimer screenshotTimer;
    QTimer secondsTimer;
    QTime secondsTime;
    QNetworkAccessManager manager;
    QJsonArray apps;
};

#endif // HOME_H
