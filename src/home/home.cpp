#include "home.h"
#include "ui_home.h"

Home::Home(Store& store, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home), store(store)
{
    ui->setupUi(this);
}

Home::~Home()
{
    delete ui;
}
