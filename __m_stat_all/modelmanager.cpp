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

HintModel *ModelManager::hintModel() const
{
    return _hintModel;
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

void ModelManager::setHintModel(HintModel *hintModel)
{
    if ( _hintModel == hintModel ) {
        return;
    }

    _hintModel = hintModel;
    emit hintModelChanged(_hintModel);
}
