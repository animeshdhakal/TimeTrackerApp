#include "login.h"
#include "./ui_login.h"

Login::Login(Store &store, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Login), store(store)
{
    ui->setupUi(this);

    QObject::connect(&manager, &QNetworkAccessManager::finished, this, &Login::onLoginResponse);
}

Login::~Login()
{
    delete ui;
}

void Login::onLoginResponse(QNetworkReply *reply)
{
    QVariant qStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = qStatusCode.toInt();
    QString json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    if (qStatusCode == 200)
    {
        if (!jsonObject["server"].isNull())
        {            
            store.set("server", jsonObject["server"].toString());

            QString email = ui->email->text();
            QString password = ui->password->text();

            QJsonObject json;

            json.insert("email", email);
            json.insert("password", password);

            QJsonDocument doc(json);

            request.setUrl(QUrl(store.get("server").toString() + "/api/user/login"));

            request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

            manager.post(request, doc.toJson(QJsonDocument::Compact));

            return;
        }

        log.info("Using Server: " + store.get("server").toString());

        log.info("Login successful, Opening Home Page");

        Notification::show("TimeTracker", (std::string("Logged in as ") + ui->email->text().toStdString()).c_str());

        this->store.set("token", jsonObject["token"].toString());

        this->close();

        Home *home = new Home(this->store);

        home->setFixedSize(home->size());

        home->setAttribute(Qt::WA_DeleteOnClose);

        home->show();
    }
    else
    {
        log.info("Login Failed: " + jsonObject["message"].toString());
        QMessageBox::warning(this, "Login", jsonObject["message"].toString());
    }
}

void Login::on_login_clicked()
{
    if(ui->email->text().isEmpty() || ui->password->text().isEmpty() || ui->company_id->text().isEmpty())
    {
        QMessageBox::warning(this, "Login", "Please enter a valid email and password");
        return;
    }

    request.setUrl(QUrl("http://timetracker.orcacorp.net.au/getserver/"+ui->company_id->text()));

    manager.get(request);
}
