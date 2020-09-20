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

    /*!
     * \brief   Была обновлена модель, представляющая
     *          блоками содержимое покупок каждого дня.
     *          Нужно обновить данные в GUI PurchaseView
     */
    void    dailyModelReloaded();

};

#endif // SIGNALMANAGER_H
