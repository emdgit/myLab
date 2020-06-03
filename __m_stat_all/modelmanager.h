#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>

#include "purchasegroupmodel.h"

class ModelManager : public QObject
{

    Q_PROPERTY(PurchaseGroupModel* profitModel
               READ profitModel
               WRITE setProfitModel
               NOTIFY profitModelChanged)

    Q_PROPERTY(PurchaseGroupModel* spendModel
               READ
               spendModel
               WRITE setSpendModel
               NOTIFY spendModelChanged)

    Q_OBJECT

public:

    explicit ModelManager(QObject *parent = nullptr);

    PurchaseGroupModel* profitModel() const;
    PurchaseGroupModel* spendModel() const;


public slots:

    void setProfitModel(PurchaseGroupModel* profitModel);
    void setSpendModel(PurchaseGroupModel* spendModel);


signals:

    void profitModelChanged(PurchaseGroupModel* profitModel);
    void spendModelChanged(PurchaseGroupModel* spendModel);


private:

    PurchaseGroupModel* _profitModel;
    PurchaseGroupModel* _spendModel;
};

#endif // MODELMANAGER_H
