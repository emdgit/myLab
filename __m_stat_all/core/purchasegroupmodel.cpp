#include "purchasegroupmodel.h"

PurchaseGroupModel::PurchaseGroupModel(PGStorage * st, QObject * parent) :
    QAbstractItemModel (parent), _st(st) {}

QModelIndex PurchaseGroupModel::index(int row, int column, const QModelIndex & parent) const
{
    Q_UNUSED(column);

    auto p_index = toPNodeIndex(parent);
    auto p_index_child = (*p_index) + row;
    auto pair = _index_set.insert( p_index_child );

    auto raw_ptr = const_cast<PNodeIndex*>( &(*pair.first) );

    return createIndex( row, 1, raw_ptr );
}

QModelIndex PurchaseGroupModel::parent(const QModelIndex & child) const
{
    if ( !child.isValid() ) {
        return QModelIndex();
    }

    auto p_index = toPNodeIndex(child);

    if ( !*p_index ) {
        return QModelIndex();
    }

    auto p_index_parent = (*p_index)--;

    auto it = _index_set.find( p_index_parent );

    if ( it == _index_set.end() ) {
        throw std::runtime_error( "Index not found. For some reason..." );
    }

    auto raw_ptr = const_cast<PNodeIndex*>( &(*it) );

    return createIndex( (*it).back(), 1, raw_ptr );
}

int PurchaseGroupModel::rowCount(const QModelIndex & parent) const
{
    if ( !parent.isValid() ) {
        return _st->childCount({});
    }

    auto p_index = toPNodeIndex( parent );

    return _st->childCount( *p_index );
}

int PurchaseGroupModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent)

    return 1;
}

QVariant PurchaseGroupModel::data(const QModelIndex & index, int role) const
{
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if ( !index.isValid() ) {
        return QVariant();
    }

    auto p_index = toPNodeIndex(index);
    auto node = _st->node( *p_index );

    if ( !node ) {
        return QVariant();
        // todo warning or exeption
    }

    return node->_data->name().data();
}

PNodeIndex *PurchaseGroupModel::toPNodeIndex(const QModelIndex & index) const noexcept
{
    return static_cast<PNodeIndex*>( index.internalPointer() );
}
