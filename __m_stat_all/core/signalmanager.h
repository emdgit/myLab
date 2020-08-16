#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>

class SignalManager : public QObject
{

    Q_OBJECT

public:
    explicit SignalManager(QObject *parent = nullptr);

    void    reloadDailyModel();


signals:

    void    dailyModelReloaded();

};

#endif // SIGNALMANAGER_H
