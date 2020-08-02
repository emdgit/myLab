#include "purchasemodeldaily.h"
#include "purchase.h"
#include "storage.h"

PurchaseModelDaily::PurchaseModelDaily(QObject *parent)
    : QObject(parent)
{
}

using st_t = std::vector<Purchase*>;

// check for ptrs aren't null & vectors aren't empty
bool checkSt(st_t *s1, st_t *s2)
{
    if (!s1 && !s2) {
        return false;
    }

    if (s1->empty() && s2->empty()) {
        return false;
    }

    return true;
}

int PurchaseModelDaily::days() const
{
    return static_cast<int>(dates_.size());
}

int PurchaseModelDaily::count(int day) const
{
    const auto &node = map_[static_cast<size_t>(day)];

    int res(0);

    if (node.s_begin != -1) {
        res += (node.s_end - node.s_begin + 1);
    }

    if (node.p_begin != -1) {
        res += (node.p_end - node.p_begin + 1);
    }

    return res;
}

QString PurchaseModelDaily::name(int day, int row) const
{
    const auto &node = map_[static_cast<size_t>(day)];

    Purchase * p;

    if (row <= node.s_end - node.s_begin) {
        p = ST.purchasesSpend()->at(static_cast<size_t>(row));
    } else {
        row -= (node.s_end - node.s_begin + 1);
        p = ST.purchasesProfit()->at(static_cast<size_t>(row));
    }

    return QString::fromStdString(p->name());
}

void PurchaseModelDaily::reloadMap()
{
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();

    dates_.clear();
    map_.clear();

    for (const auto &p : *st_s) {
        dates_.insert(p->lastDate());
    }

    for (const auto &p : *st_p) {
        dates_.insert(p->lastDate());
    }

    auto date = startDate();
    auto date_f = finishFate();

    size_t i(0), j(0);

    map_node node;
    memset(&node, -1, sizeof (map_node));

    while (date >= date_f) {
        node.s_begin = static_cast<int>(i);
        node.p_begin = static_cast<int>(j);

        if (!st_s->empty()) {
            while (i < st_s->size() && (*st_s)[i]->lastDate() == date) {
                node.s_end = static_cast<int>(i);
                ++i;
            }
        }

        if (node.s_end == -1) {
            node.s_begin = -1;
        }

        if (!st_p->empty()) {
            while (j < st_p->size() && (*st_p)[j]->lastDate() == date) {
                node.p_end = static_cast<int>(j);
                ++j;
            }
        }

        if (node.p_end == -1) {
            node.p_begin = -1;
        }

        if (node.p_begin != -1 || node.s_begin != -1) {
            map_.push_back(node);
        }

        memset(&node, -1, sizeof (map_node));
        date = date.addDays(-1);
    }
}

QDate PurchaseModelDaily::startDate() const
{
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();

    if (!checkSt(st_p, st_s)) {
        return {};
    }

    if (st_s->empty()) {
        return  st_p->front()->lastDate();
    } else if (st_p->empty()) {
        return st_s->front()->lastDate();
    } else {
        return std::max(st_s->front()->lastDate(),
                        st_p->front()->lastDate());
    }
}

QDate PurchaseModelDaily::finishFate() const
{
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();

    if (!checkSt(st_p, st_s)) {
        return {};
    }

    if (st_s->empty()) {
        return  st_p->back()->lastDate();
    } else if (st_p->empty()) {
        return st_s->back()->lastDate();
    } else {
        return std::min(st_s->back()->lastDate(),
                        st_p->back()->lastDate());
    }
}

