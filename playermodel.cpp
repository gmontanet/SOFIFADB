#include "playermodel.h"

playerModel::playerModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    //Inicialização do modelo
    this->model = new QStandardItemModel(0, 13, this);

    //Headers da tabela (titulo da coluna)
    const char *headers[] = {
      "soFIFA ID",
      "Apelido",
      "Nome",
      "Nacionalidade",
      "Altura (cm)",
      "Peso (kg)",
      "Idade",
      "Clube",
      "Posições",
      "Overhall",
      "Avaliação",
      "Total de votos",
      "Nota do Usuário"
    };

    //Inicialização dos headers
    for(int i = 0; i < 13; i++)
        this->model->setHeaderData(i,Qt::Horizontal, QObject::tr(headers[i]));
}

void playerModel::addToTopOfList(methods::playerData* player)
{
    //Insere linha no primeiro espaço
    this->model->insertRow(0);

    //ID SOFIFA
    this->model->setData(this->model->index(0 ,0), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,0), player->sofifa_id);

    //APELIDO
    this->model->setData(this->model->index(0 ,1), player->short_name.c_str());

    //NOME
    this->model->setData(this->model->index(0 ,2), player->long_name.c_str());

    //NACIONALIDADE
    this->model->setData(this->model->index(0 ,3), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,3), player->nationality.c_str());

    //ALTURA
    this->model->setData(this->model->index(0 ,4), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,4), player->height_cm);

    //PESO
    this->model->setData(this->model->index(0 ,5), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,5), player->weight_kg);

    //IDADE
    this->model->setData(this->model->index(0 ,6), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,6), player->age);

    //CLUBE
    this->model->setData(this->model->index(0 ,7), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,7), player->club_name.c_str());

    //POSIÇÕES
    this->model->setData(this->model->index(0 ,8), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,8), player->positions.c_str());

    //OVERALL
    this->model->setData(this->model->index(0 ,9), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,9), player->overall);

    //AVALIAÇÃO
    this->model->setData(this->model->index(0 ,10), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,10), player->rating);

    //TOTAL DE VOTOS
    this->model->setData(this->model->index(0 ,11), Qt::AlignCenter, Qt::TextAlignmentRole);
    this->model->setData(this->model->index(0 ,11), player->total_votes);
}

void playerModel::addMultOnList(std::vector<methods::playerData *> playerList, size_t rows, std::vector<float> ratingList)
{
    size_t size = playerList.size();

    this->model->insertRows(0, rows);

    if(!ratingList.size()) ratingList = std::vector<float>(size, 0.0);

    for(size_t i = 0, j = size-1; i < rows; i++, j--)
    {
        //ID SOFIFA
        this->model->setData(this->model->index(i ,0), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,0), playerList.at(j)->sofifa_id);

        //APELIDO
        this->model->setData(this->model->index(i ,1), playerList.at(j)->short_name.c_str());

        //NOME
        this->model->setData(this->model->index(i ,2), playerList.at(j)->long_name.c_str());

        //NACIONALIDADE
        this->model->setData(this->model->index(i ,3), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,3), playerList.at(j)->nationality.c_str());

        //ALTURA
        this->model->setData(this->model->index(i ,4), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,4), playerList.at(j)->height_cm);

        //PESO
        this->model->setData(this->model->index(i ,5), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,5), playerList.at(j)->weight_kg);

        //IDADE
        this->model->setData(this->model->index(i ,6), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,6), playerList.at(j)->age);

        //CLUBE
        this->model->setData(this->model->index(i ,7), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,7), playerList.at(j)->club_name.c_str());

        //POSIÇÕES
        this->model->setData(this->model->index(i ,8), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,8), playerList.at(j)->positions.c_str());

        //OVERALL
        this->model->setData(this->model->index(i ,9), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,9), playerList.at(j)->overall);

        //AVALIAÇÃO
        this->model->setData(this->model->index(i ,10), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,10), playerList.at(j)->rating);

        //TOTAL DE VOTOS
        this->model->setData(this->model->index(i ,11), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,11), playerList.at(j)->total_votes);

        //AVALIAÇÃO DO USUÁRIO
        this->model->setData(this->model->index(i ,12), Qt::AlignCenter, Qt::TextAlignmentRole);
        this->model->setData(this->model->index(i ,12), ratingList.at(j));
    }
}

QAbstractItemModel* playerModel::getModel(void)
{
    return this->model;
}




QVariant playerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

QModelIndex playerModel::index(int row, int column, const QModelIndex &parent) const
{
    // FIXME: Implement me!
}

QModelIndex playerModel::parent(const QModelIndex &index) const
{
    // FIXME: Implement me!
}

int playerModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int playerModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant playerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();




    // FIXME: Implement me!
    return QVariant();
}
