#ifndef ERRORDIALOG_H
#define ERRORDIALOG_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
class errorDialog;
}

class errorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit errorDialog(uint8_t errorCode, QWidget *parent = nullptr);
    ~errorDialog();

private:

    Ui::errorDialog *ui;
};

#endif // ERRORDIALOG_H
