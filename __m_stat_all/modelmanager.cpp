#include "modelmanager.h"

ModelManager::ModelManager(QObject *parent) : QObject(parent)
{
    _purchaseModel = new PurchaseModel(false);
}

PurchaseGroupModel *ModelManager::profitModel() const
{
    return _profitModel;
}

PurchaseGroupModel *ModelManager::spendModel() const
{
    return _spendModel;
}

PurchaseModel *ModelManager::purchaseModel() const
{
    return _purchaseModel;
}

HintModel *ModelManager::hintModel() const
{
    return _hintModel;
}

PurchaseModelDaily *ModelManager::purchaseModelDaily() const
{
    return _purchaseModelDaily;
}

PeriodModel *ModelManager::periodModel() const
{
    return _periodModel;
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

void ModelManager::setPurchaseModel(PurchaseModel * purchaseModel)
{
    if ( _purchaseModel == purchaseModel ) {
        return;
    }

    _purchaseModel = purchaseModel;
    emit purchaseModelChanged(_purchaseModel);
}

void ModelManager::setPurchaseModelDaily(PurchaseModelDaily *purchaseModelDaily)
{
    if (_purchaseModelDaily == purchaseModelDaily)
        return;

    _purchaseModelDaily = purchaseModelDaily;
    emit purchaseModelDailyChanged(_purchaseModelDaily);
}

void ModelManager::setPeriodModel(PeriodModel *periodModel)
{
    if (_periodModel == periodModel)
        return;

    _periodModel = periodModel;
    emit periodModelChanged(_periodModel);
}
