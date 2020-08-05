#include "signalmanager.h"

SignalManager::SignalManager(QObject *parent) : QObject(parent)
{

}

void SignalManager::emitPurchaseAdd()
{
    emit purchaseAdded();
}
