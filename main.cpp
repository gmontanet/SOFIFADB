#include "mainwindow.h"
#include "loadingwindow.h"
#include "errordialog.h"
#include "methods.h"
#include <Windows.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    //Prints direcionados ao console
    if (argc > 1 and !std::strcmp(argv[1],"debug"))
    {
        //Console filho
        AllocConsole();

        //Direcionamento stdout e stderr
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);

        //Define console para prints em UTF-8
        SetConsoleOutputCP( CP_UTF8 );
    }

    //Inicialização do QT
    QApplication app(argc, argv);

    //Definição do tema
    app.setStyle("fusion");

    //Armazenamento dos dados
    methods::dataSet data;

    //Variável para contagem do tempo de execução
    uint8_t timeInSeccounds;

    //Variável para controle de erros
    uint8_t errorCode;

    //Inicializa tela de loading
    LoadingWindow loading;

    //Exibe em tela
    loading.show();

    //Inicializa sequência de load
    errorCode  = loading.load(&timeInSeccounds, data);

    //Fecha janela de loading
    loading.close();

    //Caso houver erro durante o loading
    if(errorCode)
    {
        //Som de alerta
        app.beep();

        //Inicialização janela de erro
        errorDialog error(errorCode);

        //Exibe e aguarda finalização da janela de erro
        error.show();
        error.exec();

        //Finaliza o programa
        exit(EXIT_SUCCESS);
    }

    //Caso não haja erros

    //Inicializa janela principal
    MainWindow w(timeInSeccounds, data);

    //Exibe janela
    w.show();

    //Execução do programa
    return app.exec();
}
