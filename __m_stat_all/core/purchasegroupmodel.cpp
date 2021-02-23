#include "purchasegroupmodel.h"

using namespace std;

PurchaseGroupModel::PurchaseGroupModel(QObject * parent) :
    QAbstractListModel(parent) {}

PurchaseGroupModel::PurchaseGroupModel(PGStorage * st, QObject * parent) :
    QAbstractListModel (parent), _st(st) {}

PurchaseGroupModel::~PurchaseGroupModel()
{
    _node_proection.clear();
    for (auto &n : _node_list) {
        delete n;
    }
}

int PurchaseGroupModel::rowCount(const QModelIndex & parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return _node_proection.size();
}

QVariant PurchaseGroupModel::data(const QModelIndex & index, int role) const
{
    if (!index.isValid() || role != Name) {
        return QVariant();
    }

    return groupName(index.row());
}

QString PurchaseGroupModel::groupName(int row) const
{
    if (row >= rowCount({}) || row < 0) {
        return "";
    }

    auto n = _node_proection[row];
    return QString::fromStdString(_st->node(n->index)->_data->name());
}

bool PurchaseGroupModel::hasUnderGroup(int row) const
{
    if (row >= rowCount({}) || row < 0) {
        return false;
    }

    auto n = _node_proection[row];
    return !n->children.empty();
}

int PurchaseGroupModel::depth(int row) const
{
    if (row >= rowCount({}) || row < 0) {
        return 0;
    }

    auto n = _node_proection[row];
    return n->index.size() - 1;
}

void PurchaseGroupModel::expand(int row)
{
    if (row >= rowCount({}) || row < 0) {
        return;
    }

    // Required row is not "expanded". Guaranteed by GUI.
    auto n = _node_proection[row];

    n->expand();
    auto rows = n->rowCount() - 1;

    beginInsertRows({}, row + 1, row + rows);
    updateProetcion();
    endInsertRows();
}

void PurchaseGroupModel::collapse(int row)
{
    if (row >= rowCount({}) || row < 0) {
        return;
    }

    // Required row is "expanded". Guaranteed by GUI.
    auto n = _node_proection[row];
    auto rows = n->rowCount() - 1;

    beginRemoveRows({}, row + 1, row + rows);
    n->collapse();
    updateProetcion();
    endRemoveRows();
}

bool PurchaseGroupModel::isExpanded(int row) const
{
    if (row >= rowCount({}) || row < 0) {
        return false;
    }

    auto n = _node_proection[row];
    return static_cast<bool>(n->expand_flag);
}

int PurchaseGroupModel::groupId(int row) const
{
    if (row < 0) {
        return 0;
    }
    auto n = _node_proection[row];
    return _st->node(n->index)->_data->id();
}

PurchaseGroupModel::NodeMeta * PurchaseGroupModel::node(int row) const
{
    if (row >= rowCount({}) || row < 0) {
        return nullptr;
    }

    int from(0);
    NodeMeta * meta = const_cast<NodeMeta*>(_node_list.front());

    while (true) {
        if (row <= from + meta->rowCount() - 1) {
            if (!(row - from)) {
                return const_cast<NodeMeta*>(meta);
            } else {
                ++from;
                meta = meta->children.front();
            }
        } else {
            from += meta->rowCount();
            meta = siblingNode(meta);
        }
    }
}

PurchaseGroupModel::NodeMeta *
PurchaseGroupModel::siblingNode(PurchaseGroupModel::NodeMeta * n) const
{
    const NodeList * lst = n->parent ? &n->parent->children
                                     : &_node_list;
    auto it = std::find_if(lst->begin(), lst->end(),
                           [&](const auto &nl){
        return !(*nl < *n) && !(*n < *nl);
    });

    if (it == lst->end()) {
        throw runtime_error("Cannot find given node");
    }

    ++it;

    if (it == lst->end()) {
        return nullptr;
    }

    return const_cast<NodeMeta*>(*it);
}

void PurchaseGroupModel::updateProetcion(NodeMeta * n)
{
    if (!n) {
        _node_proection.clear();

        if (_node_list.empty()) {
            return;
        }
    }

    auto *p = n ? n : _node_list.front();

    _node_proection.push_back(p);

    if (p->expand_flag) {
        if (!p->children.empty()) {
            updateProetcion(p->children.front());
        }
    }

    p = siblingNode(p);

    if (p) {
        updateProetcion(p);
    }
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
        auto n = new NodeMeta{_st, {i}, 0};
        _node_list.push_back(n);
    }

    updateProetcion();
}





PurchaseGroupModel::NodeMeta::NodeMeta(PurchaseGroupModel::NodeMeta::st_t * st,
                                       const PurchaseGroupModel::NodeMeta::index_t & ind,
                                       int depth) :
    st(st), index(ind), depth(depth), expand_flag(0)
{
    const auto s = st->childCount(index);

    for (int i(0); i < s; ++i) {
        auto child = new NodeMeta(st, index + i, depth + 1);
        addChild(child);
    }
}

PurchaseGroupModel::NodeMeta::~NodeMeta()
{
    for (auto &ch : children) {
        delete ch;
    }
}

int PurchaseGroupModel::NodeMeta::rowCount() const
{
    if (!expand_flag) {
        return 1;
    }
    if (children.empty()) {
        return 1;
    }

    int rows(1);

    for (const auto &ch : children) {
        rows += ch->rowCount();
    }

    return rows;
}

void PurchaseGroupModel::NodeMeta::expand() noexcept
{
    expand_flag = 1;
}

void PurchaseGroupModel::NodeMeta::collapse() noexcept
{
    expand_flag = 0;
}

void PurchaseGroupModel::NodeMeta::addChild(PurchaseGroupModel::NodeMeta::node_t * n)
{
    children.push_back(n);
    n->parent = this;
}

bool PurchaseGroupModel::NodeMeta::operator<(const NodeMeta & other) const
{
    return index < other.index;
}
