#include "errordialog.h"
#include "ui_errordialog.h"
#include "methods.h"

errorDialog::errorDialog(uint8_t errorCode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::errorDialog)
{
    //Setup UI
    ui->setupUi(this);

    //Titulo da janela de dialogo
    setWindowTitle(tr("Erro!"));

    //Mensagens de erro
    const char* errorMsg[] = {
        "",
        "Não foi informado uma pasta valida.",
        "Arquivo \"players_21.csv\" não encontrado.",
        "Arquivo \"rating.csv\" não  encontrado.",
        "Arquivo \"tags.csv\" não  encontrado.",
        "Falha ao abrir arquivo \"players_21.csv\".",
        "Arquivo \"players_21.csv\" diferente do esperado.",
        "Falha ao abrir arquivo \"rating.csv\".",
        "Arquivo \"rating.csv\" diferente do esperado.",
        "Falha ao abrir arquivo \"tags.csv\".",
        "Arquivo \"tags.csv\" diferente do esperado."
    };

    //Define msg de erro
    ui->errorMsg->setText(errorMsg[errorCode]);

    //LOG
    std::wcout << methods::wcurrentTime() << " Erro: "
               << errorMsg[errorCode]
               << std::endl;

    //Define imagem de exclamação
    QPixmap pixMap("media\\error.png");
    pixMap = pixMap.scaled( QSize(50,50), Qt::KeepAspectRatio, Qt::SmoothTransformation );
    ui->errorImg->setPixmap(pixMap);
    ui->errorImg->show();

    //Cria um alerta para o botão de OK
    connect(ui->okButton, &QPushButton::clicked, this, [=](){ this->close();});
}

errorDialog::~errorDialog()
{
    delete ui;
}
