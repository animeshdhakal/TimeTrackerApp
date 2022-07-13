#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include "../store/store.h"

namespace Ui {
class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

public:
    explicit Home(Store&, QWidget *parent = nullptr);
    ~Home();

private:
    Ui::Home *ui;
    Store& store;
};

#endif // HOME_H
