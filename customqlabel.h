// QLabel customizado para controle de click, entrada e saida do mouse

#ifndef CUSTOMQLABEL_H
#define CUSTOMQLABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class CustomQLabel : public QLabel
{
    Q_OBJECT

public:
    CustomQLabel(QWidget* parent = nullptr) : QLabel(parent){ }

signals:
    void clicked();
    void mouseEnter();
    void mouseLeave();

protected:

    void mousePressEvent(QMouseEvent* event) override;

    void enterEvent(QEnterEvent *event) override;

    void leaveEvent(QEvent *event) override;
};

#endif // CUSTOMQLABEL_H
