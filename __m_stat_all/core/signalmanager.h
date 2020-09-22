#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <QObject>

class SignalManager : public QObject
{

    Q_OBJECT

public:
    explicit SignalManager(QObject *parent = nullptr);

    /// Сымитировать сигнал 'dailyModelReloaded()'
    void    reloadDailyModel();

    /// Сымитировать сигнал 'purchaseAdded()'
    void    purchaseAdd();

    /// Сымитировать сигнал 'lastPeriodPurchaseAdded()'
    void    lastPeriodPurchaseAdd();


signals:

    /*!
     * \brief   Была обновлена модель, представляющая
     *          блоками содержимое покупок каждого дня.
     *          Нужно обновить данные в GUI PurchaseView
     */
    void    dailyModelReloaded();

    /*!
     * \brief   В базу данных была добавлена запись о
     *          новой покупке
     */
    void    purchaseAdded();

    /*!
     * \brief Вызывается при добавлении записи в последний период.
     */
    void    lastPeriodPurchaseAdded();

};

#endif // SIGNALMANAGER_H
