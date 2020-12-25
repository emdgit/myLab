#include "purchasemodel.h"
#include "purchase.h"
#include "storage.h"

PurchaseModel::PurchaseModel(QObject * parent) :
    QAbstractListModel (parent)
{

}

PurchaseModel::PurchaseModel(bool profit, QObject *parent)
    : QAbstractListModel(parent)
{
    setProfit(profit);
}

int PurchaseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() || !_purchase_vec) {
        return 0;
    }

    return static_cast<int>(_purchase_vec->size());
}

QVariant PurchaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const auto row = static_cast<size_t>(index.row());

    switch (role) {
        case Name: {
            auto name = (*_purchase_vec)[row]->name();
            return  QString::fromStdString(name);
        }

        case Summ: {
            return (*_purchase_vec)[row]->summ();
        }

        case LastDate: {
            return (*_purchase_vec)[row]->lastDate().toString("dd.MMM.yyyy");
        }

        default: return QVariant();
    }
}

QHash<int, QByteArray> PurchaseModel::roleNames() const
{
    QHash<int, QByteArray> hash;

    hash.insert(Name, "PName");
    hash.insert(Summ, "PSumm");
    hash.insert(LastDate, "PLastDate");

    return hash;
}

bool PurchaseModel::profit() const
{
    return _profit;
}

void PurchaseModel::reloadData()
{
    beginResetModel();
    endResetModel();
}

void PurchaseModel::setProfit(bool profit)
{
    if (_profit == profit)
        return;

    _profit = profit;

    _purchase_vec = _profit ? ST.purchasesProfitSumm()
                            : ST.purchasesSpendSumm();

    emit profitChanged(_profit);
}
