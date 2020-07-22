#include "purchasegroupmodel.h"

#include <iostream>

using namespace std;

PurchaseGroupModel::PurchaseGroupModel(QObject * parent) :
    QAbstractItemModel (parent), _st(nullptr)
{
}

PurchaseGroupModel::PurchaseGroupModel(PGStorage * st, QObject * parent) :
    QAbstractItemModel (parent), _st(st) {}

QModelIndex PurchaseGroupModel::index(int row, int column, const QModelIndex & parent) const
{
    Q_UNUSED(column);

    if ( !parent.isValid() ) {
        PNodeIndex index = _showRoot ? PNodeIndex()
                                     : PNodeIndex({row});

        auto raw_ptr = insertAndGetRawPtr( index );
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
        if ( _showRoot ) {
            return createIndex(0, 1, insertAndGetRawPtr({}));
        }
        return QModelIndex();
    }

    auto row = p_index->back();
    auto p_index_parent = (*p_index);
    p_index_parent.popBack();

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
        if ( _showRoot ) {
            return 1;
        }
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

    if ( !*p_index && _showRoot && role == Name ) {
        return _rootName;
    }

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

PNodeIndex *PurchaseGroupModel::insertAndGetRawPtr(PNodeIndex index) const {
    auto pair = _index_set.insert( index );
    auto raw_ptr = const_cast<PNodeIndex*>( &(*pair.first) );
    return raw_ptr;
}

QHash<int, QByteArray> PurchaseGroupModel::roleNames() const
{
    QHash<int, QByteArray> hash;

    hash.insert( Name, "r_pgroup_name" );

    return hash;
}

int PurchaseGroupModel::groupId(const QModelIndex &index) const
{
    auto pIndex = toPNodeIndex( index );

    if ( !pIndex ) {
        return -1;
    }

    auto node = _st->node( *pIndex );

    if ( !node ) {
        return 0;
    }

    return node->_data->id();
}

bool PurchaseGroupModel::showRoot() const
{
    return _showRoot;
}

void PurchaseGroupModel::reloadData()
{
    beginResetModel();
    endResetModel();
}

QString PurchaseGroupModel::rootName() const
{
    return _rootName;
}

void PurchaseGroupModel::setShowRoot(bool showRoot)
{
    if (_showRoot == showRoot)
        return;

    beginResetModel();
    _showRoot = showRoot;
    endResetModel();

    emit showRootChanged(_showRoot);
}

void PurchaseGroupModel::setRootName(QString rootName)
{
    if (_rootName == rootName)
        return;

    _rootName = rootName;
    emit rootNameChanged(_rootName);
}
