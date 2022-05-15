#include "loadingwindow.h"
#include "ui_loadingwindow.h"

#include "methods.h"

LoadingWindow::LoadingWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoadingWindow)
{
    //Setup da interface grafica
    ui->setupUi(this);

    //Titulo da janela
    setWindowTitle(tr("soFIFA database"));

    //Bloqueia redefinição do tamanho da janela
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint);
}

LoadingWindow::~LoadingWindow()
{
    delete ui;
}

uint8_t LoadingWindow::load(uint8_t *timeInSeccounds, methods::dataSet &data)
{
    //Controle de retorno
    uint8_t errorCode;

    //Atualiza informação na tela
    ui->label->setText("Aguardando entrada do usuário ...");

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Aguardando usuário informar uma pasta."
               << std::endl;

    //Pegar caminho das tabelas
    errorCode = LoadingWindow::getPath();
    //Tratar erro se houver
    if(errorCode) return errorCode;

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Pasta informada pelo usuário: " << this->path.toStdWString()
               << std::endl;

    //Verificar se os arquivos necessarios estão na pasta informada
    errorCode = LoadingWindow::filesVerify();
    //Tratar erro se houver
    if(errorCode) return errorCode;

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Os arquivos necessários estão presente na pasata informada."
               << std::endl;

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Inicio do contador para calcular tempo total de execução."
               << std::endl;

    //Começa do contador de tempo decorrido
    QElapsedTimer timer;
    timer.start();

    //Atualiza informação na tela
    ui->label->setText("Processando o arquivo \"players_21.csv\" ...");
    ui->label->repaint();

    //Processa e carrega no set de dados as informações do arquivo players_21.csv
    errorCode = methods::LoadAndProcessPlayers(this->path.toStdString(), data);
    //Tratar erro se houver
    if(errorCode) return errorCode;

    //Atualiza informação na tela
    ui->label->setText("Processando o arquivo \"rating.csv\" ...");
    ui->label->repaint();

    //Processa e carrega no set de dados as informações do arquivo rating.csv
    errorCode = methods::LoadAndProcessRating(this->path.toStdString(), data);
    //Tratar erro se houver
    if(errorCode) return errorCode;

    //Atualiza informação na tela
    ui->label->setText("Processando o arquivo \"tags.csv\" ...");
    ui->label->repaint();

    //Processa e carrega no set de dados as informações do arquivo tags.csv
    errorCode = methods::LoadAndProcessTags(this->path.toStdString(), data);
    //Tratar erro se houver
    if(errorCode) return errorCode;

    //Atualiza informação na tela
    ui->label->setText("Aguarde ...");
    ui->label->repaint();

    //Ordena vetor com todos jogadores com base no rating
    data.vecRating = data.vecOverhall;
    methods::radixRating(data.vecRating);

    //Inverte os vetores
    //methods::invertVector(data.vecOverhall);
    //methods::invertVector(data.vecRating);

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Encerramento do contador para calcular tempo total de execução."
               << std::endl;

    //Finaliza cronometro de tempo decorrido
    *timeInSeccounds = timer.elapsed()/1000;

    //LOG
    std::wcout << methods::wcurrentTime() << " Loading: "
               << "Tempo decorrido durante carregamento: " << *timeInSeccounds << " segundos."
               << std::endl;

    return 0;
}

uint8_t LoadingWindow::getPath()
{
    //Pegar caminho dos arquivos
    this->path = QFileDialog::getExistingDirectory(this, tr("Informe a localização das tabelas"),"/home",
                                                   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    //Verificação de cancelamento
    if(this->path == "")
    {
        return 1;
    }

    return 0;
}

uint8_t LoadingWindow::filesVerify()
{
    //arquivo players_21.csv
    QFileInfo verifyExistence(this->path, "players_21.csv");

    if(!(verifyExistence.exists() and verifyExistence.isFile())) return 2;

    //arquivo rating.csv
    verifyExistence = QFileInfo(this->path, "rating.csv");

    if(!(verifyExistence.exists() and verifyExistence.isFile())) return 3;

    //arquivo tags.csv
    verifyExistence = QFileInfo(this->path, "tags.csv");

    if(!(verifyExistence.exists() and verifyExistence.isFile())) return 4;

    return 0;
}
