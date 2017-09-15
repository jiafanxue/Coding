#include "qrcodedialog.h"
#include "ui_qrcodedialog.h"

QRcodeDialog::QRcodeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QRcodeDialog)
{
    ui->setupUi(this);
}

QRcodeDialog::~QRcodeDialog()
{
    delete ui;
}
