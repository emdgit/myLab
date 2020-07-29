#ifndef PURCHASEMODEL_H
#define PURCHASEMODEL_H

#include <QAbstractListModel>

class Purchase;

class PurchaseModel : public QAbstractListModel
{

    Q_OBJECT

    Q_PROPERTY(bool profit
               READ profit
               WRITE setProfit
               NOTIFY profitChanged)

    using purchase_vec = std::vector<Purchase*>;

    enum Roles {
        Name,
        Summ,
        LastDate
    };

public:

    explicit PurchaseModel(QObject *parent = nullptr);
    explicit PurchaseModel(bool profit, QObject *parent = nullptr);

    int         rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant    data(const QModelIndex &index,
                     int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    bool        profit() const;

    void        reloadData();


public slots:

    void        setProfit(bool profit);


signals:

    void        profitChanged(bool profit);


private:

    purchase_vec * _purchase_vec = nullptr;

    bool        _profit;

};

#endif // PURCHASEMODEL_H
