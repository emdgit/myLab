#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>

#include "hintmodel.h"
#include "purchasemodel.h"
#include "purchasegroupmodel.h"
#include "purchasemodeldaily.h"

class ModelManager : public QObject
{

    Q_PROPERTY(PurchaseGroupModel* profitModel
               READ profitModel
               WRITE setProfitModel
               NOTIFY profitModelChanged)

    Q_PROPERTY(PurchaseGroupModel* spendModel
               READ spendModel
               WRITE setSpendModel
               NOTIFY spendModelChanged)

    Q_PROPERTY(HintModel* hintModel
               READ hintModel
               WRITE setHintModel
               NOTIFY hintModelChanged)

    Q_PROPERTY(PurchaseModel* purchaseModel
               READ purchaseModel
               WRITE setPurchaseModel
               NOTIFY purchaseModelChanged)

    Q_PROPERTY(PurchaseModelDaily* purchaseModelDaily
               READ purchaseModelDaily
               WRITE setPurchaseModelDaily
               NOTIFY purchaseModelDailyChanged)

    Q_OBJECT

public:

    explicit ModelManager(QObject *parent = nullptr);

    PurchaseGroupModel* profitModel() const;
    PurchaseGroupModel* spendModel() const;
    PurchaseModel*      purchaseModel() const;
    HintModel*          hintModel() const;
    PurchaseModelDaily* purchaseModelDaily() const;

public slots:

    void setProfitModel(PurchaseGroupModel* profitModel);
    void setSpendModel(PurchaseGroupModel* spendModel);
    void setHintModel(HintModel *hintModel);
    void setPurchaseModel(PurchaseModel *purchaseModel);
    void setPurchaseModelDaily(PurchaseModelDaily* purchaseModelDaily);

signals:

    void profitModelChanged(PurchaseGroupModel* profitModel);
    void spendModelChanged(PurchaseGroupModel* spendModel);
    void hintModelChanged(HintModel* hintModel);
    void purchaseModelChanged(PurchaseModel* purchaseModel);
    void purchaseModelDailyChanged(PurchaseModelDaily* purchaseModelDaily);


private:

    PurchaseGroupModel* _profitModel        = nullptr;
    PurchaseGroupModel* _spendModel         = nullptr;
    HintModel*          _hintModel          = nullptr;
    PurchaseModel*      _purchaseModel      = nullptr;
    PurchaseModelDaily* _purchaseModelDaily = nullptr;
};

#endif // MODELMANAGER_H
