#include "purchasemodeldaily.h"
#include "purchase.h"
#include "storage.h"

#include <iostream>

PurchaseModelDaily::PurchaseModelDaily(QObject *parent)
    : QObject(parent)
{
}

PurchaseModelDaily::~PurchaseModelDaily()
{
    clear();
}

int PurchaseModelDaily::days() const
{
    return static_cast<int>(days_.size());
}

int PurchaseModelDaily::count(int day) const
{
    const auto d = static_cast<size_t>(day);
    return static_cast<int>(days_[d]->data_.size());
}

QString PurchaseModelDaily::name(int day, int row) const
{
    return QString::fromStdString(dayBlock(day, row)->name);
}

QString PurchaseModelDaily::summ(int day, int row) const
{
    return QString::number(dayBlock(day, row)->summ);
}

QString PurchaseModelDaily::date(int day) const
{
    auto it = days_.begin();
    std::advance(it, day);
    return (*it)->date.toString("dd.MM.yyyy");
}

bool PurchaseModelDaily::isProfit(int day, int row) const
{
    return dayBlock(day, row)->is_profit;
}

void PurchaseModelDaily::reloadMap()
{
    auto st_s = ST.purchasesSpend();
    auto st_p = ST.purchasesProfit();

    clear();

    if (st_s->empty() && st_p->empty()) {
        return;
    }

    auto find_day_info = [&](const QDate &d) {
        auto it = std::find_if(days_.begin(), days_.end(), [&](day_info *di){
                  return di->date == d;
        });

        return it == days_.end() ? nullptr : *it;
    };

    auto add = [&](Purchase *p, bool profit) {
        auto d_info = find_day_info(p->lastDate());

        if (!d_info) {
            days_.push_back(new day_info(p->lastDate()));
            days_.back()->add(p, profit);
        } else {
            d_info->add(p, profit);
        }
    };

    for (const auto &p : *st_s) {
        add(p, false);
    }

    for (const auto &p : *st_p) {
        add(p, true);
    }

    std::sort(days_.begin(), days_.end(), [](day_info *l, day_info *r){
        return l->date < r->date;
    });
}

PurchaseModelDaily::day_info *PurchaseModelDaily::dayInfo(int day) const
{
    const auto d = static_cast<size_t>(day);
    return days_[d];
}

PurchaseModelDaily::day_block *PurchaseModelDaily::dayBlock(int day, int row) const
{
    const auto r = static_cast<size_t>(row);
    return dayInfo(day)->data_[r];
}

void PurchaseModelDaily::clear()
{
    for (auto &di : days_) {
        delete di;
    }

    days_.clear();
}




// DAY_BLOCK Functions
PurchaseModelDaily::day_block::day_block(const std::string & name, bool is_profit) :
    name(name), is_profit(is_profit)
{
}

void PurchaseModelDaily::day_block::add(Purchase * p)
{
    block_data.push_back(p);
    summ += p->summ();
}

bool PurchaseModelDaily::day_block::is_complex() const noexcept
{
    return block_data.size() > 1;
}


// DAY_INFO Functions
PurchaseModelDaily::day_info::day_info(const QDate & d) :
    date(d)
{
}

PurchaseModelDaily::day_info::~day_info()
{
    for (auto &inf : data_) {
        delete inf;
    }
}

void PurchaseModelDaily::day_info::add(Purchase * p, bool is_profit)
{
    auto it = std::find_if(data_.begin(), data_.end(), [&](day_block *block){
              return block->name == p->name() &&
                     block->is_profit == is_profit;
    });

    if (it == data_.end()) {
        data_.push_back(new day_block(p->name(), is_profit));
        auto &block = data_.back();
        block->add(p);
    } else {
        (*it)->add(p);
    }

    using db = day_block;

    std::sort(data_.begin(), data_.end(), [](db *l, db*r){
        if (!(l->is_profit ^ r->is_profit)) {
            return l->summ > r->summ;
        } else if (l->is_profit) {
            return true;
        } else {
            return false;
        }
    });
}
