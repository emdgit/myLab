#include "recordbygroupmodel.h"
#include "purchaserecord.h"
#include "storage.h"

static bool findGroup(int id, PGStorage *st) {
    auto s = st->childCount({});

    if (!s) {
        return false;
    }

    std::function<bool(PNodeIndex)> searchReq = [&](PNodeIndex index) -> bool {
        if (st->node(index)->_data->id() == id) {
            return true;
        }
        auto s = st->childCount(index);
        for (int i(0); i < s; ++i) {
            if (st->node(index + i)->_data->id() != id) {
                return true;
            }
        }
        return false;
    };

    PNodeIndex index{};

    for (int i(0); i < s; ++i) {
        if (searchReq(index + i)) {
            return true;
        }
    }

    return false;
}

RecordByGroupModel::RecordByGroupModel(QObject *parent)
    : QObject(parent)
{
    records_spend_ = ST.recordsSpend();
    records_profit_ = ST.recordsProfit();
}

QStringList RecordByGroupModel::model() const
{
    return {};
}

void RecordByGroupModel::setGroupId(int id)
{
    if (id == group_id_) {
        return;
    }

    group_id_ = id;
    is_profit_ = isProfit(group_id_);
    record_indexes_.clear();

    auto vec = is_profit_ ? records_profit_
                          : records_spend_;

    QStringList names;

    for (size_t i(0); i < vec->size(); ++i) {
        if ((*vec)[i]->group_id() == group_id_) {
            record_indexes_.push_back(static_cast<int>(i));
            names << QString::fromStdString((*vec)[i]->name());
        }
    }
}

int RecordByGroupModel::rowCount() const noexcept
{
    if (group_id_ < 0) {
        return 0;
    }

    return record_indexes_.size();
}

int RecordByGroupModel::groupId() const noexcept
{
    return group_id_;
}

QString RecordByGroupModel::recordName(int row) const
{
    if (row >= rowCount() || row < 0 || group_id_ < 0) {
        return {};
    }

    auto rec = record(row);
    return QString::fromStdString(rec->name());
}

bool RecordByGroupModel::isProfit(int groupId) const
{
    auto cache_it = profit_cache_.find(groupId);

    if (cache_it != profit_cache_.end()) {
        return (*cache_it).second;
    }

    if (findGroup(groupId, ST.groupsProfit())) {
        profit_cache_[groupId] = true;
        return true;
    }

    if (findGroup(groupId, ST.groupsSpend())) {
        profit_cache_[groupId] = false;
        return false;
    }

    throw std::runtime_error("Given groupID doesn't registered"
                             "in any data");
}

PurchaseRecord * RecordByGroupModel::record(int row) const
{
    auto vec = is_profit_ ? records_profit_
                          : records_spend_;

    auto i = static_cast<size_t>(record_indexes_[row]);
    return (*vec)[i];
}

