#include "login.h"
#include "./ui_login.h"

Login::Login(Store& store, Home& home, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login), store(store), home(home)
{
    ui->setupUi(this);

    this->manager = new QNetworkAccessManager();

    QObject::connect(manager, &QNetworkAccessManager::finished, this, &Login::onLoginResponse);
}

Login::~Login()
{
    delete ui;
    delete manager;
}

void Login::onLoginResponse(QNetworkReply* reply)
{
    QVariant qStatusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    int statusCode = qStatusCode.toInt();
    QString json = reply->readAll();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject jsonObject = jsonDocument.object();

    if(qStatusCode == 200){
        this->store.set("token", jsonObject["token"].toString());

        this->close();
        
        this->home.show();
    }else{
        QMessageBox::warning(this, "Login", jsonObject["message"].toString());
    }
}


void Login::on_login_clicked()
{
    QString email = ui->email->text();
    QString password = ui->password->text();

    if(email.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Please fill all the fields");
        return;
    }
    
    QJsonObject json;

    json.insert("email", email);
    json.insert("password", password);

    QJsonDocument doc(json);

    request.setUrl(QUrl(store.get("server").toString() + "/api/user/login"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->post(request, doc.toJson());
}

