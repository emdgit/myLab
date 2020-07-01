#ifndef PURCHASEGROUPMODEL_H
#define PURCHASEGROUPMODEL_H

#include <QAbstractItemModel>

#include <set>

#include "storagedefinition.h"

/// Иерархичная модель отображения групп
class PurchaseGroupModel : public QAbstractItemModel
{

    using IndexSet = std::set<PNodeIndex>;

    enum GroupRole {
        Name = Qt::UserRole    /// Имя группы. Роль
    };

    Q_OBJECT

public:
    PurchaseGroupModel( QObject * parent = nullptr );
    PurchaseGroupModel( PGStorage *st, QObject * parent = nullptr );

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;

    int         rowCount(const QModelIndex & parent) const override;
    int         columnCount(const QModelIndex & parent) const override;

    QVariant    data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE
    int         groupId( const QModelIndex &index ) const;


protected:

    PNodeIndex *toPNodeIndex( const QModelIndex &index ) const noexcept;


private:

    PGStorage * _st;

    /// Задействованые объекты PNodeIndex.
    /// Используются как internalPointer внутри QModelIndex
    mutable IndexSet _index_set;

};

#endif // PURCHASEGROUPMODEL_H
