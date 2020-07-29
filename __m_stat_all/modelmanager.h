#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>

#include "hintmodel.h"
#include "purchasemodel.h"
#include "purchasegroupmodel.h"

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

    Q_OBJECT

public:

    explicit ModelManager(QObject *parent = nullptr);

    PurchaseGroupModel* profitModel() const;
    PurchaseGroupModel* spendModel() const;
    PurchaseModel*      purchaseModel() const;
    HintModel*          hintModel() const;


public slots:

    void setProfitModel(PurchaseGroupModel* profitModel);
    void setSpendModel(PurchaseGroupModel* spendModel);
    void setHintModel(HintModel *hintModel);
    void setPurchaseModel(PurchaseModel *purchaseModel);


signals:

    void profitModelChanged(PurchaseGroupModel* profitModel);
    void spendModelChanged(PurchaseGroupModel* spendModel);
    void hintModelChanged(HintModel* hintModel);
    void purchaseModelChanged(PurchaseModel* purchaseModel);


private:

    PurchaseGroupModel* _profitModel    = nullptr;
    PurchaseGroupModel* _spendModel     = nullptr;
    HintModel*          _hintModel      = nullptr;
    PurchaseModel*      _purchaseModel  = nullptr;
};

#endif // MODELMANAGER_H
