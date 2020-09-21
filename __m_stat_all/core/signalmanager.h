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

};

#endif // SIGNALMANAGER_H
