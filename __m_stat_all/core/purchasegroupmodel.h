#ifndef PURCHASEGROUPMODEL_H
#define PURCHASEGROUPMODEL_H

#include <QAbstractItemModel>

#include "storagedefinition.h"

class PurchaseGroupModel : public QAbstractItemModel
{
public:
    PurchaseGroupModel( QObject * parent = nullptr );

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;

    int         rowCount(const QModelIndex & parent) const override;
    int         columnCount(const QModelIndex & parent) const override;

    QVariant    data(const QModelIndex & index, int role) const override;


private:

    PGStorage * _st;

};

#endif // PURCHASEGROUPMODEL_H
