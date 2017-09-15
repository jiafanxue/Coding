#ifndef LOGININTERFACE_H
#define LOGININTERFACE_H

#include <QWidget>

namespace Ui {
class LoginInterface;
}

class LoginInterface : public QWidget
{
    Q_OBJECT

public:
    explicit LoginInterface(QWidget *parent = 0);
    ~LoginInterface();

private:
    Ui::LoginInterface *ui;
};

#endif // LOGININTERFACE_H
