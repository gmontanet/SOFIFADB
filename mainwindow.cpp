#include "mainwindow.h"
#include "ui_mainwindow.h"

//Constructor
MainWindow::MainWindow(uint64_t timeInSeccounds, methods::dataSet &data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // =============================== Inicializações ============================== //

    //Configuração da UI
    ui->setupUi(this);

    //Bloqueia redefinição do tamanho da janela
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    //Titulo
    setWindowTitle(tr("soFIFA database"));

    //Inicializa exibição como "todos"
    this->displayNumber = 0;

    //Inicializa opção de pesquisa como "nome do jogador"
    this->searchMode = MainWindow::searchTypes::playerName;

    //Inicialização do controle de sort como "ordenamento por rating"
    this->sortByOverall = false;

    //Define intervalo aceitos na caixa do top (0~99999)
    ui->topInput->setValidator(new QIntValidator(1, 99999, this));

    //Define variável apontado para set de dados
    this->data = &data;

    //Incialização ponteiro do modelo da tabela
    this->modelView = new playerModel();

    //Inicializa placeholder da barra de pesquisa
    ui->searchBox->setPlaceholderText("Digite o nome do jogdor ...");

    //Construção da mensagem de tempo
    QString time = "Tempo de inicialização: ";
    time += QString::number((uint64_t) timeInSeccounds / 60);
    time += " min e ";
    time += QString::number(timeInSeccounds % 60);
    time += " seg.";

    //Exibe tempo de construção da primeira etapa
    ui->info->setText(time);

    //Inicializa botões do menu de contexto
    this->contextMenuActInit();

    //Gif animado
    QMovie *gif = new QMovie("media\\SOFIFADB.gif");
    //std::cout << gif->isValid() << std::endl << gif->lastErrorString().toStdString() << std::endl;
    ui->gifLabel->setMovie(gif);
    auto gifScale = gif->scaledSize();  //resize
    gifScale.setHeight(70);
    gifScale.setWidth(110);
    gif->setScaledSize(gifScale);       //set resize
    gif->start();                       //start movie

    //Definição de botão customizado X
    this->label_x = new CustomQLabel(this);
    this->label_x->setGeometry(773, 94, 12, 12);
    QPixmap pixMap("media\\X.png");
    pixMap = pixMap.scaled(QSize(10,10), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->label_x->setPixmap(pixMap);
    this->label_x->hide();

    //=========================== Inicialização da tabela ===========================//

    //Inicializa vetor do conteúdo atual da tabela
    this->tableContent = this->data->vecRating;

    //Atualiza tabela
    this->tableUpdate(true);

    //Ajusta última coluna até o fim da tabela
    ui->table->horizontalHeader()->setStretchLastSection(true);

    //Bloqueio da edição dos elmentos da tabela
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Ajusta tamanho das colunas para a maior string
    for(int i = 0; i < 13; i++) ui->table->resizeColumnToContents(i);

    //Ocultar colunas extras inicialmente
    ui->table->hideColumn(1);   //Apelido
    ui->table->hideColumn(3);   //Nacionalidade
    ui->table->hideColumn(4);   //Altura
    ui->table->hideColumn(5);   //Peso
    ui->table->hideColumn(6);   //Idade
    ui->table->hideColumn(7);   //Clube
    ui->table->hideColumn(12);  //Nota do Usuário

    // =========================== Definição dos eventos =========================== //

    //Eventos dos botões de seleção de modo de pesquisa
    connect(ui->radioUserId, &QRadioButton::clicked, this, &MainWindow::searchModeUpdate);
    connect(ui->radioPlayerId, &QRadioButton::clicked, this, &MainWindow::searchModeUpdate);
    connect(ui->radioPlayerName, &QRadioButton::clicked, this, &MainWindow::searchModeUpdate);
    connect(ui->radioTags, &QRadioButton::clicked, this, &MainWindow::searchModeUpdate);
    connect(ui->radioPositions, &QRadioButton::clicked, this, &MainWindow::searchModeUpdate);

    //Eventos dos botões de modo de ordenamento
    connect(ui->radioOverall, &QRadioButton::clicked, this, &MainWindow::sortOverall);
    connect(ui->radioRating, &QRadioButton::clicked, this, &MainWindow::sortRating);

    //Eventos dos botões de exibição
    connect(ui->radioAll, &QRadioButton::clicked, this, &MainWindow::displayAll);
    connect(ui->radioTop, &QRadioButton::clicked, this, &MainWindow::displayTop);
    connect(ui->topInput, &QLineEdit::textChanged, this, &MainWindow::displayTop);

    //Evento da caixa de pesquisa
    connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);

    //Eventos associados ao botão X (apaga conteudo digitado na barra de pesquisa)
    connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::LabelXVisible);
    connect(this->label_x, &CustomQLabel::clicked, this, &MainWindow::LabelXClick);
    connect(this->label_x, &CustomQLabel::mouseEnter, this, &MainWindow::LabelXEnter);
    connect(this->label_x, &CustomQLabel::mouseLeave, this, &MainWindow::LabelXLeave);

    //Evento para menu de contexto ao clickar no header da tabela
    ui->table->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->table->horizontalHeader(), &QTableView::customContextMenuRequested, this, &MainWindow::tableHeaderContextMenu);

}

MainWindow::~MainWindow()
{
    //Exclui elementos
    delete ui;

    delete sofifa_id_act;
    delete short_name_act;
    delete long_name_act;
    delete nationality_act;
    delete height_cm_act;
    delete weight_kg_act;
    delete age_act;
    delete club_act;
    delete position_act;
    delete overall_act;
    delete rating_act;
    delete total_votes_act;
    delete user_rating_act;

    delete modelView;
}

void MainWindow::contextMenuActInit()
{
    //Coluna soFIFA ID
    sofifa_id_act = new QAction(tr("&soFIFA ID"), this);
    sofifa_id_act->setCheckable(true);

    // Obs: Coluna soFIFA ID fica desabilidade para edição afim de
    // evitar bug onde todas colunas são ocultadas e já não há cabeçalhos
    // de colunas para clicar e abrir o menu de contexto.

    sofifa_id_act->setDisabled(true);
    sofifa_id_act->setChecked(true);

    //Coluna Apelido
    short_name_act = new QAction("Apelido", this);
    short_name_act->setCheckable(true);
    short_name_act->setChecked(false);
    connect(short_name_act, &QAction::triggered, this, [=](){

        // Função anônima / lambda que verifica se o botão
        // da respectiva coluna está ativo, se estiver mostra
        // a coluna em questão, caso contrario a oculta.


        if(short_name_act->isChecked())
            ui->table->showColumn(1);
        else
            ui->table->hideColumn(1);
    });

    // Obs: A seguir o mesmo processo para a coluna Apelido se repete
    // para as demais colunas.

    //Coluna Nome
    long_name_act = new QAction("Nome", this);
    long_name_act->setCheckable(true);
    long_name_act->setChecked(true);
    connect(long_name_act, &QAction::triggered, this, [=](){

        if(long_name_act->isChecked())
            ui->table->showColumn(2);
        else
            ui->table->hideColumn(2);
    });

    //Coluna Nacionalidade
    nationality_act = new QAction("Nacionalidade", this);
    nationality_act->setCheckable(true);
    nationality_act->setChecked(false);
    connect(nationality_act, &QAction::triggered, this, [=](){

        if(nationality_act->isChecked())
            ui->table->showColumn(3);
        else
            ui->table->hideColumn(3);
    });

    //Coluna Altura
    height_cm_act = new QAction("Altura", this);
    height_cm_act->setCheckable(true);
    height_cm_act->setChecked(false);
    connect(height_cm_act, &QAction::triggered, this, [=](){

        if(height_cm_act->isChecked())
            ui->table->showColumn(4);
        else
            ui->table->hideColumn(4);
    });

    //Coluna Peso
    weight_kg_act = new QAction("Peso", this);
    weight_kg_act->setCheckable(true);
    weight_kg_act->setChecked(false);
    connect(weight_kg_act, &QAction::triggered, this, [=](){

        if(weight_kg_act->isChecked())
            ui->table->showColumn(5);
        else
            ui->table->hideColumn(5);
    });

    //Coluna Idade
    age_act = new QAction("Idade", this);
    age_act->setCheckable(true);
    age_act->setChecked(false);
    connect(age_act, &QAction::triggered, this, [=](){

        if(age_act->isChecked())
            ui->table->showColumn(6);
        else
            ui->table->hideColumn(6);
    });

    //Coluna Clube
    club_act = new QAction("Clube", this);
    club_act->setCheckable(true);
    club_act->setChecked(false);
    connect(club_act, &QAction::triggered, this, [=](){

        if(club_act->isChecked())
            ui->table->showColumn(7);
        else
            ui->table->hideColumn(7);
    });

    //Coluna Posições
    position_act = new QAction("Posições", this);
    position_act->setCheckable(true);
    position_act->setChecked(true);
    connect(position_act, &QAction::triggered, this, [=](){

        if(position_act->isChecked())
            ui->table->showColumn(8);
        else
            ui->table->hideColumn(8);
    });

    //Coluna Overall
    overall_act = new QAction("Overall", this);
    overall_act->setCheckable(true);
    overall_act->setChecked(true);
    connect(overall_act, &QAction::triggered, this, [=](){

        if(overall_act->isChecked())
            ui->table->showColumn(9);
        else
            ui->table->hideColumn(9);
    });

    //Coluna Avaliações
    rating_act = new QAction("Rating", this);
    rating_act->setCheckable(true);
    rating_act->setChecked(true);
    connect(rating_act, &QAction::triggered, this, [=](){

        if(rating_act->isChecked())
            ui->table->showColumn(10);
        else
            ui->table->hideColumn(10);
    });

    //Coluna Avaliações
    total_votes_act = new QAction("Total de votos", this);
    total_votes_act->setCheckable(true);
    total_votes_act->setChecked(true);
    connect(total_votes_act, &QAction::triggered, this, [=](){

        if(total_votes_act->isChecked())
            ui->table->showColumn(11);
        else
            ui->table->hideColumn(11);
    });

    //Coluna Avaliação do usuário
    user_rating_act = new QAction("Nota do usuário", this);
    user_rating_act->setCheckable(true);
    user_rating_act->setChecked(false);

    //Desabilitado pois só é exibido no tipo de pesquisa por usuário

    user_rating_act->setDisabled(true);
}

void MainWindow::tableHeaderContextMenu()
{
    //Menu de contexto
    QMenu menu(this);

    //Adiciona botões ao menu de contexto
    menu.addAction(sofifa_id_act);
    menu.addAction(short_name_act);
    menu.addAction(long_name_act);
    menu.addAction(nationality_act);
    menu.addAction(height_cm_act);
    menu.addAction(weight_kg_act);
    menu.addAction(age_act);
    menu.addAction(club_act);
    menu.addAction(position_act);
    menu.addAction(overall_act);
    menu.addAction(rating_act);
    menu.addAction(total_votes_act);
    menu.addAction(user_rating_act);

    //Inicializa menu de contexto na posição do mouse
    menu.exec(QCursor::pos());
}

void MainWindow::searchModeUpdate()
{

    //Apaga conteudo da barra de pesquisa
    ui->searchBox->clear();

    //atualiza o placeholder
    if(ui->radioUserId->isChecked())
    {
        //Permite que a pesquisa seja exibida dinamicamente
        disconnect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);
        disconnect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);
        connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);

        //Limita caixa de pesquisa para aceitar apenas numeros
        ui->searchBox->setValidator(new QIntValidator(0, 9999999, this));

        //atualiza o placeholder
        ui->searchBox->setPlaceholderText("Digite o ID do usuário ...");

        //Atualiza modo de pesquisa
        this->searchMode = MainWindow::searchTypes::userID;

        //LOG
        std::wcout << methods::wcurrentTime() << " Modo de pesquisa: "
                   << "USERID."
                   << std::endl;
    }

    else if(ui->radioPlayerName->isChecked())
    {
        //Permite que a pesquisa seja exibida dinamicamente
        disconnect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);
        disconnect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);
        connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);

        //Libera caixa de pesquisa para qualquer caractere
        ui->searchBox->setValidator( nullptr );

        //atualiza o placeholder
        ui->searchBox->setPlaceholderText("Digite o nome do jogdor ...");

        //Atualiza modo de pesquisa
        this->searchMode = MainWindow::searchTypes::playerName;

        //LOG
        std::wcout << methods::wcurrentTime() << " Modo de pesquisa: "
                   << "PLAYERNAME."
                   << std::endl;
    }

    else if(ui->radioTags->isChecked())
    {
        //Exige que usuário tecle enter para realizar a pesquisa
        disconnect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);
        disconnect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);
        connect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);

        //Libera caixa de pesquisa para qualquer caractere
        ui->searchBox->setValidator( nullptr );

        //atualiza o placeholder
        ui->searchBox->setPlaceholderText("Digite as tags entre apóstrofes em seguida pressione ENTER ...");

        //Atualiza modo de pesquisa
        this->searchMode = MainWindow::searchTypes::tags;

        //LOG
        std::wcout << methods::wcurrentTime() << " Modo de pesquisa: "
                   << "TAGS."
                   << std::endl;
    }

    else if(ui->radioPlayerId->isChecked())
    {
        //Permite que a pesquisa seja exibida dinamicamente
        disconnect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);
        disconnect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);
        connect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);

        //Limita caixa de pesquisa para aceitar apenas numeros
        ui->searchBox->setValidator(new QIntValidator(0, 9999999, this));

        //atualiza o placeholder
        ui->searchBox->setPlaceholderText("Digite o ID do jogador ...");

        //Atualiza modo de pesquisa
        this->searchMode = MainWindow::searchTypes::PlayerID;

        //LOG
        std::wcout << methods::wcurrentTime() << " Modo de pesquisa: "
                   << "PLAYERID."
                   << std::endl;
    }

    else if(ui->radioPositions->isChecked())
    {        
        //Exige que usuário tecle enter para realizar a pesquisa
        disconnect(ui->searchBox, &QLineEdit::textChanged, this, &MainWindow::searchHandler);
        disconnect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);
        connect(ui->searchBox, &QLineEdit::returnPressed, this, &MainWindow::searchHandler);

        //Libera caixa de pesquisa para qualquer caractere
        ui->searchBox->setValidator( nullptr );

        //atualiza o placeholder
        ui->searchBox->setPlaceholderText("Digite as posições entre apóstrofes em seguida pressione ENTER ...");

        //Atualiza modo de pesquisa
        this->searchMode = MainWindow::searchTypes::positions;

        //LOG
        std::wcout << methods::wcurrentTime() << " Modo de pesquisa: "
                   << "POSITIONS."
                   << std::endl;
    }

    return;
}

void MainWindow::searchHandler()
{
    //Pega string digitado
    std::string strIn = ui->searchBox->text().toStdString();

    //Verifica existencia de caractere
    if(!strIn.length())
    {
        //Exibi tabela completa
        if(this->sortByOverall)
            this->tableContent = this->data->vecOverhall;
        else
            this->tableContent = this->data->vecRating;

        //Atualiza tabela
        this->tableUpdate(true);

        //Update Info
        ui->info->setText(QString("Total de jogadores: %1.").arg(this->tableContent.size()));

        return;
    }

    switch (this->searchMode)
    {
        //Pesquisa pelo nome
        case MainWindow::searchTypes::playerName:
        {
            //Pesquisa na trie a string digitada
            this->tableContent = this->data->ttNames.search(strIn);

            //Ordena tabela
            if(this->sortByOverall)
                methods::radixOverall(this->tableContent);
            else
                methods::radixRating(this->tableContent);

            //Remove duplicidade
            methods::removeSequenceDuplicated(this->tableContent);

            //Atualiza tabela
            this->tableUpdate(true);

            //Atualiza informação
            auto info = QString("%1 resultados para o nome \"%2\".").arg(QString::number(this->tableContent.size()), QString(strIn.c_str()));
            ui->info->setText(info);

            break;
        }

        //Pesquisa por ID do player
        case MainWindow::searchTypes::PlayerID:
        {
            //ID do jogador, string -> int
            uint32_t sofifa_id = std::stoi(strIn);

            //Pesquisa na hashtable a string digitada
            auto player = this->data->htPlayers.search(sofifa_id);

            //Verifica se o ID foi encontrado
            if(!player)
            {
                //Atualiza informação
                auto info = QString("ID %1 não encontrado.").arg(QString::number(sofifa_id));
                ui->info->setText(info);

                //Limpa lista
                this->tableContent.clear();
                this->tableUpdate();

                return;
            }

            //Atualiza conteudo da tabela
            this->tableContent = std::vector<methods::playerData*>(1, player);

            //Atualiza tabela
            this->tableUpdate();

            //Atualiza informação
            QString info = QString("ID %1 encontrado.").arg(QString::number(sofifa_id));
            ui->info->setText(info);

            break;
        }

        //Pesquisa por ID do usuário
        case MainWindow::searchTypes::userID:
        {
            //ID do usuário, string -> int
            uint32_t user_id = std::stoi(strIn);

            //Pesquisa na hashtable a string digitada
            auto userDatalist = this->data->htUser.search(user_id);

            //Verifica se o ID foi encontrado
            if(!userDatalist.size())
            {
                //Atualiza informação
                auto info = QString("ID %1 não encontrado.").arg(QString::number(user_id));
                ui->info->setText(info);

                //Limpa lista
                this->tableContent.clear();
                this->tableUpdate();

                return;
            }

            //Atualiza conteudo da tabela
            this->tableContentUser = userDatalist;

            //Atualiza tabela
            this->tableUpdate(this->tableContentUser);

            //Atualiza informação
            QString info = QString("ID %1 encontrado.").arg(QString::number(user_id));
            ui->info->setText(info);

            break;
        }

        //Pesquisa por posições (posições == tags)
        case MainWindow::searchTypes::positions:
        {
            //Verifica se todas palavras estão entre apóstrofe
            if((methods::countChar(strIn, '\'')%2) or !methods::countChar(strIn, '\''))
            {
                AlertWindow alert("Todas as palavras devem estar entre apóstrofe.");
                alert.exec();

                return;
            }

            //Separa as posições
            auto posVec = methods::parsingTags(strIn);

            /* Verifica quantidade de tags
             * Caso maior que 1, deve-se verificar os jogadores que
             * contém as tags informadas.
             * Caso igual a 1, deve-se apenas mostrar lista retornada
             */
            if(posVec.size() > 1)
            {
                auto playerList1 = data->ttPosition.search(posVec.at(0));

                for(size_t i = 0; i < posVec.size()-1; i++)
                {
                    auto playerList2 = data->ttPosition.search(posVec.at(i+1));

                    playerList1 = methods::tagIntersection(playerList1, playerList2);
                }

                //Filtro 1000 avaliações
                for(size_t index = 0; index < playerList1.size(); index++)
                {
                    if(playerList1.at(index)->total_votes < 1000)
                    {
                        playerList1.erase(playerList1.begin()+index);
                        index--;
                    }
                }


                this->tableContent = playerList1;
                this->tableUpdate();

            } else if(posVec.size() == 1) {

                auto playerList = data->ttPosition.search(posVec.at(0));

                //Filtro 1000 avaliações
                for(size_t index = 0; index < playerList.size(); index++)
                {
                    if(playerList.at(index)->total_votes < 1000)
                    {
                        playerList.erase(playerList.begin()+index);
                        index--;
                    }
                }


                this->tableContent = playerList;
                this->tableUpdate();
            }

            //Atualiza informação
            ui->info->setText(QString("Foram encontrados %1 jogadores com a(s) posição(ões) digitada(s).").arg(this->tableContent.size()));


            break;
        }

        //Pesquisa por tags
        case MainWindow::searchTypes::tags:
        {
            //Verifica se todas palavras estão entre apóstrofe
            if((methods::countChar(strIn, '\'')%2) or !methods::countChar(strIn, '\''))
            {
                AlertWindow alert("Todas as palavras devem estar entre apóstrofe.");
                alert.exec();

                return;
            }

            //Separa as tags
            auto tagsVec = methods::parsingTags(strIn);

            /* Verifica quantidade de tags
             * Caso maior que 1, deve-se verificar os jogadores que
             * contém as multiplas tags informadas.
             * Caso igual a 1, deve-se apenas mostrar lista retornada
             */

            if(tagsVec.size() > 1)
            {
                auto playerList1 = data->ttTags.search(tagsVec.at(0));

                for(size_t i = 0; i < tagsVec.size()-1; i++)
                {
                    auto playerList2 = data->ttTags.search(tagsVec.at(i+1));

                    playerList1 = methods::tagIntersection(playerList1, playerList2);
                }

                this->tableContent = playerList1;
                this->tableUpdate();

            } else if(tagsVec.size() == 1) {

                auto playerList = data->ttTags.search(tagsVec.at(0));

                this->tableContent = playerList;
                this->tableUpdate();
            }

            //Atualiza informação
            ui->info->setText(QString("Foram encontrados %1 jogadores com a(s) tag(s) digitada(s).").arg(this->tableContent.size()));

            break;
        }

        default:
        {
            //Faça nada
        }
    }

    return;
}

void MainWindow::tableUpdate(bool preSorted)
{
    //LOG
    std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
               << "Atualização padrão."
               << std::endl;

    //Verifica se o vetor já estava ordenado previamente
    if(!preSorted)
    {        
        //Ordena o vetor pelo overall
        if(this->sortByOverall)
        {
            //LOG
            std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
                       << "Ordenamento com base no Overall."
                       << std::endl;

            //Radix
            methods::radixOverall(this->tableContent);
        }

        //Ordena o vetor pelo rating
        else
        {
            //LOG
            std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
                       << "Ordenamento com base no Rating."
                       << std::endl;

            //Radix
            methods::radixRating(this->tableContent);
        }
    }
    else
    {
        //LOG
        std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
                   << "Vetor previamente ordenado."
                   << std::endl;
    }

    //Novo modelo de tabela
    auto tempModelView = new playerModel();

    //Se o modo de exibição estiver marcado para exibir todos os resultados ou
    //a quantidade total de elementos for menor que o limite máximo de exibição
    if(this->displayNumber == 0 or (this->tableContent.size() <= this->displayNumber))
    {
        //Adiciona elementos no modelo com base na quantidade de elementos (todos elementos)
        tempModelView->addMultOnList(this->tableContent, this->tableContent.size());
    }
    else
    {
        //Adiciona elementos no modelo com base no limite máximo de exibição
        tempModelView->addMultOnList(this->tableContent, this->displayNumber);
    }

    //Atualiza exibição da tabela
    ui->table->setModel(tempModelView->getModel());

    //Oculta coluna de avaliação do usuário
    ui->table->hideColumn(12);

    //Habilita botões de modo de ordenamento
    ui->radioOverall->setEnabled(true);
    ui->radioRating->setEnabled(true);

    //Desseleciona no contexto de menu a coluna do usuário
    this->user_rating_act->setChecked(false);

    //Deleta tabela anterior
    delete this->modelView;

    //Guarda endereço do modelo da nova tabela
    this->modelView = tempModelView;

    //LOG
    std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
               << "Concluído!"
               << std::endl;
}

void MainWindow::tableUpdate(std::vector<methods::userData2> &userDataVec)
{
    //LOG
    std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
               << "Atualização exclusiva do modo USERID."
               << std::endl;

    //Ordena vetor com base na avialiação do usuário especifico
    methods::radixRating(userDataVec);

    //Dividir vetor das avaliações e player
    auto parsedList = methods::parsingUserData(userDataVec);

    //Atualiza conteudo da tabela
    this->tableContent = parsedList.first;

    //Novo modelo de tabela
    auto tempModelView = new playerModel();

    //Se o modo de exibição estiver marcado para exibir todos os resultados ou
    //a quantidade total de elementos for menor que o limite máximo de exibição
    if(this->displayNumber == 0 or (this->tableContent.size() <= this->displayNumber))
    {
        //Adiciona elementos no modelo com base na quantidade de elementos (todos elementos)
        //(Dados padrão do jogador + avaliação do usuário para cada jogador)
        tempModelView->addMultOnList(this->tableContent, this->tableContent.size(), parsedList.second);
    }
    else
    {
        //Adiciona elementos no modelo com base no limite máximo de exibição
        tempModelView->addMultOnList(this->tableContent, this->displayNumber, parsedList.second);
    }


    //Atualiza exibição da tabela
    ui->table->setModel(tempModelView->getModel());

    //Exibe coluna do rating do usuário
    ui->table->showColumn(12);

    //Ajusta tamanho da coluna
    ui->table->resizeColumnToContents(12);

    //Desabilita botões de modo de ordenamento
    ui->radioOverall->setEnabled(false);
    ui->radioRating->setEnabled(false);

    //Seleciona no contexto de menu a coluna do usuário
    this->user_rating_act->setChecked(true);

    //Deleta tabela anterior
    delete this->modelView;

    //Guarda endereço do modelo da nova tabela
    this->modelView = tempModelView;

    //LOG
    std::wcout << methods::wcurrentTime() << " Atualização da tabela: "
               << "Concluído!"
               << std::endl;
}

void MainWindow::LabelXVisible()
{
    //Se houver texto habilita o X
    if(ui->searchBox->text().length())
    {
        this->label_x->show();
    }
    //Caso contrario, desabilita
    else
        this->label_x->hide();
}

void MainWindow::LabelXClick()
{
    //Apaga conteudo da barra de pesquisa
    ui->searchBox->clear();

    //Corrige bug da atualização da tabela estar desconectada
    if(ui->radioTags->isChecked() or ui->radioPositions->isChecked())
        MainWindow::searchHandler();
}

void MainWindow::LabelXEnter()
{
    //altera imagem da label para X selecionado
    QPixmap pixMap("media\\Xselected.png");
    pixMap = pixMap.scaled(QSize(10,10), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->label_x->setPixmap(pixMap);
}

void MainWindow::LabelXLeave()
{
    //Restaura imagem padrão do X
    QPixmap pixMap("media\\X.png");
    pixMap = pixMap.scaled(QSize(10,10), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->label_x->setPixmap(pixMap);
}

void MainWindow::sortOverall()
{
    //Define ordenamento para Overall
    this->sortByOverall = true;

    //Atualiza tabela
    this->tableUpdate();
}

void MainWindow::sortRating()
{
    //Define ordenamento para Rating
    this->sortByOverall = false;

    //Atualiza tabela
    this->tableUpdate();
}

void MainWindow::displayAll()
{
    //Define limite de display para 0 (exibir todos)
    this->displayNumber = 0;

    //LOG
    std::wcout << methods::wcurrentTime() << " Total de exibição: "
               << "Exibir todos."
               << std::endl;

    //Verifica se pesquisa está para usuário
    if(ui->radioUserId->isChecked())
    {
        //Update de tabela exclusivo da pesquisa por Id do usuário
        this->tableUpdate(this->tableContentUser);
    }
    else
    {
        //Update padrão da tabela
        this->tableUpdate();
    }
}

void MainWindow::displayTop()
{
    //Verifica se o botão "Top" está selecionado
    if(ui->radioTop->isChecked())
    {
        //Define limite de itens a serem exibidos
        this->displayNumber = ui->topInput->text().toInt();

        //Se o conteudo do TopInput for nulo ou zero, definir para exibir todos
        if(!this->displayNumber)
        {
            this->displayAll();
            return;
        }

        //LOG
        std::wcout << methods::wcurrentTime() << " Total de exibição: "
                   << "Exibir até " << this->displayNumber << " elementos."
                   << std::endl;

        //Verifica se pesquisa está para usuário
        if(ui->radioUserId->isChecked())
        {
            //Update de tabela exclusivo da pesquisa por Id do usuário
            this->tableUpdate(this->tableContentUser);
        }
        else
        {
            //Update padrão da tabela
            this->tableUpdate();
        }
    }
}
