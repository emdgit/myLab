#ifndef STORAGEDEFINITION_H
#define STORAGEDEFINITION_H

#include "hierarchicalstorage.h"
#include "purchasegroup.h"

/*
 * Здесь хранятся псевдонимы типов для разных типов Storage
*/

using PGStorage = HierarchicalStorage<PurchaseGroup,int,&PurchaseGroup::id>;

#endif // STORAGEDEFINITION_H
