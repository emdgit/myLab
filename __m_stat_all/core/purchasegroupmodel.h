#ifndef PURCHASEGROUPMODEL_H
#define PURCHASEGROUPMODEL_H

#include <QAbstractItemModel>

#include <set>

#include "storagedefinition.h"

class PurchaseGroupModel : public QAbstractItemModel
{

    using IndexSet = std::set<PNodeIndex>;

public:
    PurchaseGroupModel( PGStorage *st, QObject * parent = nullptr );

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;

    int         rowCount(const QModelIndex & parent) const override;
    int         columnCount(const QModelIndex & parent) const override;

    QVariant    data(const QModelIndex & index, int role) const override;


protected:

    PNodeIndex *toPNodeIndex( const QModelIndex &index ) const noexcept;


private:

    PGStorage * _st;

    /// Задействованые объекты PNodeIndex.
    /// Используются как internalPointer внутри QModelIndex
    mutable IndexSet _index_set;

};

Q_DECLARE_METATYPE(PurchaseGroupModel*)

#endif // PURCHASEGROUPMODEL_H
