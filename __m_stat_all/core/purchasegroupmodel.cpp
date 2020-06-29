#include "purchasegroupmodel.h"

#include <iostream>

using namespace std;

PurchaseGroupModel::PurchaseGroupModel(QObject * parent) :
    QAbstractItemModel (parent)
{
}

PurchaseGroupModel::PurchaseGroupModel(PGStorage * st, QObject * parent) :
    QAbstractItemModel (parent), _st(st) {}

QModelIndex PurchaseGroupModel::index(int row, int column, const QModelIndex & parent) const
{
    Q_UNUSED(column);

    if ( !parent.isValid() ) {
        auto pair = _index_set.insert( {row} );
        auto raw_ptr = const_cast<PNodeIndex*>( &(*pair.first) );

        return createIndex( row, column, raw_ptr );
    }

    auto p_index = toPNodeIndex(parent);
    auto p_index_child = (*p_index) + row;
    auto pair = _index_set.insert( p_index_child );

    auto raw_ptr = const_cast<PNodeIndex*>( &(*pair.first) );

    return createIndex( row, column, raw_ptr );
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

    if ( p_index->size() == 1 ) {
        // Его предок - корень
        return QModelIndex();
    }

    auto row = p_index->back();
    auto p_index_parent = (*p_index);
    p_index_parent--;

    auto it = _index_set.find( p_index_parent );

    if ( it == _index_set.end() ) {
        throw std::runtime_error( "Index not found. For some reason..." );
    }

    auto raw_ptr = const_cast<PNodeIndex*>( &(*it) );

    return createIndex( row, 1, raw_ptr );
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
    if ( !index.isValid() ) {
        return QVariant();
    }

    auto p_index = toPNodeIndex(index);
    auto node = _st->node( *p_index );

    if ( !node ) {
        return QVariant();
    }

    switch (role) {
        case Name: {
            return QString(node->_data->name().data());
        }
    }

    return QVariant();
}

PNodeIndex *PurchaseGroupModel::toPNodeIndex(const QModelIndex & index) const noexcept
{
    return static_cast<PNodeIndex*>( index.internalPointer() );
}

QVariant PurchaseGroupModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if ( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if ( orientation == Qt::Horizontal ) {
        return "OWL goes faster";
    }

    return section + 1;
}

QHash<int, QByteArray> PurchaseGroupModel::roleNames() const
{
    QHash<int, QByteArray> hash;

    hash.insert( Name, "r_pgroup_name" );

    return hash;
}
