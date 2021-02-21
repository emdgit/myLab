#include "purchasegroupmodel.h"

#include <iostream>

using namespace std;

PurchaseGroupModel::PurchaseGroupModel(QObject * parent) :
    QAbstractListModel(parent)
{

}

PurchaseGroupModel::PurchaseGroupModel(PGStorage * st, QObject * parent) :
    QAbstractListModel (parent), _st(st)
{
}

int PurchaseGroupModel::rowCount(const QModelIndex & parent) const
{
    cout << "rowCount()" << endl;
    if (parent.isValid()) {
        return 0;
    }

    int res(0);

    for (const auto &nm : _node_list) {
        res += nm.rowCount();
    }

    return res;
}

QVariant PurchaseGroupModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid() || role != Name) {
        return QVariant();
    }

    auto nm = node(index.row());
    auto data = _st->node(nm->index)->_data;

    return QString::fromStdString(data->name());
}

QString PurchaseGroupModel::groupName(int row) const
{
    auto n = node(row);
    return QString::fromStdString(_st->node(n->index)->_data->name());
}

bool PurchaseGroupModel::hasUnderGroup(int row) const
{
    auto n = node(row);
    return !n->children.empty();
}

int PurchaseGroupModel::depth(int row) const
{
    auto n = node(row);
    return n->index.size() - 1;
}

void PurchaseGroupModel::expand(int row)
{
    (void)row;
}

void PurchaseGroupModel::collapse(int row)
{
    (void)row;
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

PurchaseGroupModel::NodeMeta * PurchaseGroupModel::node(int row) const
{
    if (row >= rowCount({})) {
        return nullptr;
    }

    auto sibling = [&](const NodeMeta *n) {
        const NodeList * lst = n->parent ? &n->parent->children
                                         : &_node_list;
        auto it = std::find_if(lst->begin(), lst->end(),
                               [&](const auto &nl){
            return !(nl < *n) && !(*n < nl);
        });

        if (it == lst->end()) {
            throw runtime_error("Cannot find sibling");
        }

        ++it;

        if (it == lst->end()) {
            throw runtime_error("Cannot find sibling");
        }

        return &*it;
    };

    int from(0);
    const NodeMeta * meta = &_node_list.front();

    while (true) {
        if (row <= from + meta->rowCount() - 1) {
            if (!(row - from)) {
                return const_cast<NodeMeta*>(meta);
            } else {
                ++from;
                meta = &meta->children.front();
            }
        } else {
            from += meta->rowCount();
            meta = sibling(meta);
        }
    }
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

int PurchaseGroupModel::depth(const QModelIndex & index) const
{
    return toPNodeIndex(index)->size() - 1;
}

void PurchaseGroupModel::reloadData()
{
    beginResetModel();
    endResetModel();
}

void PurchaseGroupModel::init()
{
    const auto s = _st->childCount({});

    for (int i(0); i < s; ++i) {
        _node_list.push_back({_st, {i}, 0});
    }
}





PurchaseGroupModel::NodeMeta::NodeMeta(PurchaseGroupModel::NodeMeta::st_t * st,
                                       const PurchaseGroupModel::NodeMeta::index_t & ind,
                                       int depth, node_t * parent) :
    st(st), parent(parent), index(ind), depth(depth), rows_under(0), expand_flag(0)
{
    const auto s = st->childCount(index);

    for (int i(0); i < s; ++i) {
        children.push_back({st, index + i, depth + 1, this});
    }

    rows_under = children.size();
}

int PurchaseGroupModel::NodeMeta::rowCount() const
{
    if (!expand_flag) {
        return 1;
    }
    if (children.empty()) {
        return 1;
    }

    return 1 + rows_under;
}

int PurchaseGroupModel::NodeMeta::expand()
{
    if (children.empty() || expand_flag) {
        return 0;
    }

    expand_flag = 1;

    int delta_rows(0);

    for (const auto &ch : children) {
        delta_rows += ch.rows_under + 1;
    }

    rows_under += delta_rows;

    auto *p = parent;

    while (p) {
        p->rows_under += delta_rows;
        p = p->parent;
    }

    return delta_rows;
}

int PurchaseGroupModel::NodeMeta::collapse()
{
    if (children.empty() || !expand_flag) {
        return 0;
    }

    auto delta_rows = rows_under;

    expand_flag = 0;
    rows_under = 0;

    auto *p = parent;

    while (p) {
        p->rows_under -= delta_rows;
        p = p->parent;
    }

    return delta_rows;
}

bool PurchaseGroupModel::NodeMeta::operator<(const NodeMeta & other) const
{
    return index < other.index;
}
