#ifndef PLAYERMODEL_H
#define PLAYERMODEL_H

#include <QAbstractItemModel>
#include <QStandardItemModel>

#include <vector>

#include <methods.h>

class playerModel : public QAbstractItemModel
{
    Q_OBJECT

public:

    //Constructor
    explicit playerModel(QObject *parent = nullptr);

    //Adiciona jogador no inicio da tabela
    void addToTopOfList(methods::playerData* player);

    void addMultOnList(std::vector<methods::playerData*> playerList, size_t rows, std::vector<float> = std::vector<float>());

    //Retorna o modelo da classe
    QAbstractItemModel* getModel(void);





    //Coisas criadas pelo QT  ¯\_(ツ)_/¯

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:

    //modelo padrão dos dados
    QStandardItemModel *model;



};

#endif // PLAYERMODEL_H
