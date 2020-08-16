#include "signalmanager.h"

SignalManager::SignalManager(QObject *parent) : QObject(parent)
{

}

void SignalManager::currentPeriodPurchaseAdd()
{
    emit currentPeriodPurchaseAdded();
}

void SignalManager::reloadDailyModel()
{
    emit dailyModelReloaded();
}
