#ifndef QRCODEDIALOG_H
#define QRCODEDIALOG_H

#include <QDialog>

namespace Ui {
class QRcodeDialog;
}

class QRcodeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QRcodeDialog(QWidget *parent = 0);
    ~QRcodeDialog();

private:
    Ui::QRcodeDialog *ui;
};

#endif // QRCODEDIALOG_H
