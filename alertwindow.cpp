#include "alertwindow.h"
#include "ui_alertwindow.h"

AlertWindow::AlertWindow(const char* alert, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AlertWindow)
{
    //setup
    ui->setupUi(this);

    //titulo
    setWindowTitle(tr("Aviso!"));

    //Bloqueia redefinição do tamanho da janela
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    //Define imagem de exclamação
    QPixmap pixMap("media\\error.png");
    pixMap = pixMap.scaled( QSize(50,50), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    ui->label_2->setPixmap(pixMap);
    ui->label_2->show();

    //Define aviso
    ui->label->setText(alert);

    //Evento para botão OK
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){ this->close(); });
    //connect(ui->pushButton, &QPushButton::clicked, this, &AlertWindow::okButtom);

    //Beep
    QApplication::beep();
}

AlertWindow::~AlertWindow()
{
    delete ui;
}


