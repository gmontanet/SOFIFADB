#ifndef LOADINGWINDOW_H
#define LOADINGWINDOW_H

#include <QDialog>

#include <QFileDialog>      //Pegar pasta dos arquivos
#include <QDateTime>        //Pegar hora atual
#include <QElapsedTimer>    //Contador de tempo

#include <methods.h>

namespace Ui {
class LoadingWindow;
}

class LoadingWindow : public QDialog
{
    Q_OBJECT

public:

    //Construtor: recebe a variavel de retorno do tempo de execução e o dataSet
    explicit LoadingWindow(QWidget *parent = nullptr);

    //Destructor
    ~LoadingWindow();

    // ETAPA 1: Carregar arquivos e construir as estruturas de dados
    uint8_t load(uint8_t *timeInSeccounds, methods::dataSet &data);

private:

    //Interface
    Ui::LoadingWindow *ui;

    //Caminho para arquivos
    QString path;

    //Solicitar pasta dos arquivos ao usuário
    uint8_t getPath();

    //Verifica a existencia dos arquivos na pasta informada
    uint8_t filesVerify();
};

#endif // LOADINGWINDOW_H
