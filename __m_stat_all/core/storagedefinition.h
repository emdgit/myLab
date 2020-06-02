#ifndef STORAGEDEFINITION_H
#define STORAGEDEFINITION_H

#include "hierarchicalstorage.h"
#include "purchasegroup.h"

using PGStorage = HierarchicalStorage<PurchaseGroup,int,&PurchaseGroup::id>;

#endif // STORAGEDEFINITION_H
