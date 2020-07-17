#include "hintmodel.h"
#include "purchaserecord.h"

HintModel::HintModel(QObject *parent) : QObject(parent)
{
}

QStringList HintModel::model() const
{
    return _model;
}

void HintModel::setRecords(HintModel::RecordVec *records)
{
    _records = records;
}

bool HintModel::containsRecord(const QString &rec) const
{
    auto it = std::find_if(_records->begin(), _records->end(),
                           [&](PurchaseRecord * const &r){
              return QString(r->name().data()).compare(rec, Qt::CaseInsensitive) == 0;
    });

    return it != _records->end();
}

void HintModel::setModel(QStringList lst)
{
    _model = lst;

    emit modelChanged(_model);
}

void HintModel::setHintFactor(QString str)
{
    _hintFactor = str;
    updateModel();
}

void HintModel::updateModel()
{
    if ( _hintFactor.isEmpty() || !_records ) {
        return;
    }

    QStringList lst;

    for ( const auto &rec : *_records ) {
        QString str(rec->name().data());

        if ( str.startsWith( _hintFactor, Qt::CaseInsensitive ) ) {
            lst << str;
        }
    }

    lst.sort( Qt::CaseInsensitive );

    setModel(lst);
}
