#include "purchasemodeldaily.h"
#include "purchase.h"
#include "storage.h"

#include <QDate>

PurchaseModelDaily::PurchaseModelDaily(QObject *parent)
    : QObject(parent)
{
}

using st_t = std::vector<Purchase*>;

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
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();

    if (!checkSt(st_p, st_s)) {
        return 0;
    }

    return static_cast<int>(finishFate().daysTo(startDate()));
}

int PurchaseModelDaily::count(int day) const
{

}

QString PurchaseModelDaily::name(int day, int row) const
{

}

void PurchaseModelDaily::reloadMap()
{
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();
    auto date = startDate();
    auto s_it = st_s->begin();
    auto p_it = st_p->begin();

    map_node node;

    while (true) {

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

