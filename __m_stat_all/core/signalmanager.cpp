#include "signalmanager.h"

SignalManager::SignalManager(QObject *parent) : QObject(parent)
{

}

void SignalManager::reloadDailyModel()
{
    emit dailyModelReloaded();
}
