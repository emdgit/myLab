#include "modelmanager.h"

ModelManager::ModelManager(QObject *parent) : QObject(parent)
{

}

PurchaseGroupModel *ModelManager::profitModel() const
{
    return _profitModel;
}

PurchaseGroupModel *ModelManager::spendModel() const
{
    return _spendModel;
}

void ModelManager::setProfitModel(PurchaseGroupModel * profitModel)
{
    if (_profitModel == profitModel)
        return;

    _profitModel = profitModel;
    emit profitModelChanged(_profitModel);
}

void ModelManager::setSpendModel(PurchaseGroupModel * spendModel)
{
    if (_spendModel == spendModel)
        return;

    _spendModel = spendModel;
    emit spendModelChanged(_spendModel);
}
