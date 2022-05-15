#ifndef ALERTWINDOW_H
#define ALERTWINDOW_H

#include <QDialog>
#include <QApplication>

namespace Ui {
class AlertWindow;
}

class AlertWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AlertWindow(const char* alert, QWidget *parent = nullptr);
    ~AlertWindow();



private:
    Ui::AlertWindow *ui;
};

#endif // ALERTWINDOW_H
