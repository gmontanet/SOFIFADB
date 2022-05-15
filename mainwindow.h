#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMovie>
#include <QFileSystemModel>
#include <QPixmap>
#include <QMenu>
#include <QMenuBar>
#include <QContextMenuEvent>

#include <playermodel.h>
#include <customqlabel.h>
#include <methods.h>
#include <alertwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(uint64_t, methods::dataSet &, QWidget *parent = nullptr);
    ~MainWindow();

private:

    //Interface
    Ui::MainWindow *ui;

    //Label customizada para botão X
    CustomQLabel *label_x;

    //Enumeração para modos de pesquisa
    enum searchTypes
    {
        playerName,
        PlayerID,
        userID,
        tags,
        positions
    };

    //Controle do modo de pesquisa atual
    MainWindow::searchTypes searchMode;

    //Controle do numero de itens a exibir
    size_t displayNumber;

    //Controle do tipo de ordenamento, true: overall, false: rating
    bool sortByOverall;

    //Ponteiro para o set de dados
    methods::dataSet* data;

    //Conteúdo da tabela
    std::vector<methods::playerData*> tableContent;

    //Conteúdo da tabela caso especial UserID
    std::vector<methods::userData2> tableContentUser;

    //
    playerModel *modelView;

    //Menu de contexto
    QAction *sofifa_id_act;
    QAction *short_name_act;
    QAction *long_name_act;
    QAction *nationality_act;
    QAction *height_cm_act;
    QAction *weight_kg_act;
    QAction *age_act;
    QAction *club_act;
    QAction *position_act;
    QAction *overall_act;
    QAction *rating_act;
    QAction *total_votes_act;
    QAction *user_rating_act;

    //Exibição do menu de contexto do cabeçalho horizontal da tabela
    void tableHeaderContextMenu();

    //Incializa opções do menu de contexto do cabeçalho horizontal da tabela
    void contextMenuActInit();


    //Atualiza da tabela
    void tableUpdate(bool = false);

    //Atualiza da tabela exclusiva para modo USERID
    void tableUpdate(std::vector<methods::userData2> &);

    //Atualiza modo de pesquisa
    void searchModeUpdate(void);

    //Reaaliza a pesquisa desejada
    void searchHandler(void);

    //Atualiza modo de ordenamento
    void sortOverall(void);
    void sortRating(void);

    //Botão X
    void LabelXVisible(void);
    void LabelXClick(void);
    void LabelXEnter(void);
    void LabelXLeave(void);

    //Total de exibições
    void displayAll(void);
    void displayTop(void);
};

#endif // MAINWINDOW_H
