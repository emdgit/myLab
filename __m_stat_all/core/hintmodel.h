#ifndef HINTMODEL_H
#define HINTMODEL_H

#include <QObject>

#include <vector>

class PurchaseRecord;

/// Модель предоставляет подсказки при вводе записи в GUI.
class HintModel : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QStringList model
               READ model
               WRITE setModel
               NOTIFY modelChanged)

    using RecordVec = std::vector<PurchaseRecord*>;

public:
    explicit HintModel(QObject *parent = nullptr);

    QStringList         model() const;

    void                setRecords( RecordVec * records );

    Q_INVOKABLE
    bool                containsRecord( const QString &rec ) const;


public slots:

    void                setModel( QStringList lst );
    void                setHintFactor( QString str );


signals:

    void                modelChanged(QStringList model);


protected:

    void                updateModel();


private:

    QString             _hintFactor;

    QStringList         _model;

    RecordVec *         _records = nullptr;

};

#endif // HINTMODEL_H
