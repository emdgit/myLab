#ifndef PURCHASEGROUP_H
#define PURCHASEGROUP_H

#include <string>
#include <vector>
#include <memory>

#include "iobject.h"

/// Класс-обертка над группой покупки
class PurchaseGroup : public IObject
{

    enum class GroupType : unsigned short {
        Profit, // Группа доходов
        Spend   // Группа расходов
    };

public:

    PurchaseGroup();
    PurchaseGroup( const int &id, const int &pId,
                   const std::string &name, GroupType type = GroupType::Spend ) noexcept;
    PurchaseGroup(const PurchaseGroup&) = default;

    int                 id() const noexcept;
    const int &         parentId() const noexcept;

    const std::string   name() const noexcept;

    void                setId(int id) noexcept;
    void                setParentId(int parentId) noexcept;
    void                setName(const std::string &name) noexcept;

    // IObject interface
    void                fromPgAnswer( pg::Answer * answer, unsigned long i ) override;


private:

    int                 _id;
    int                 _parentId;

    std::string         _name;

    GroupType           _type = GroupType::Spend;

};

using PurchaseGroupPtr = std::shared_ptr<PurchaseGroup>;
using GroupVecS = std::vector<PurchaseGroupPtr>;

#endif // PURCHASEGROUP_H
