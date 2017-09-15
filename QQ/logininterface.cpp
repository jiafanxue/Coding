#include "logininterface.h"
#include "ui_logininterface.h"

LoginInterface::LoginInterface(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginInterface)
{
    ui->setupUi(this);
}

LoginInterface::~LoginInterface()
{
    delete ui;
}
