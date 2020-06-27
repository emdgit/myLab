#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>

#include "purchasegroupmodel.h"
#include "hintmodel.h"

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

    Q_OBJECT

public:

    explicit ModelManager(QObject *parent = nullptr);

    PurchaseGroupModel* profitModel() const;
    PurchaseGroupModel* spendModel() const;

    HintModel*          hintModel() const;


public slots:

    void setProfitModel(PurchaseGroupModel* profitModel);
    void setSpendModel(PurchaseGroupModel* spendModel);
    void setHintModel(HintModel *hintModel);


signals:

    void profitModelChanged(PurchaseGroupModel* profitModel);
    void spendModelChanged(PurchaseGroupModel* spendModel);
    void hintModelChanged(HintModel* hintModel);


private:

    PurchaseGroupModel* _profitModel = nullptr;
    PurchaseGroupModel* _spendModel  = nullptr;
    HintModel*          _hintModel   = nullptr;
};

#endif // MODELMANAGER_H
