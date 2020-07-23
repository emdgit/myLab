#ifndef PERIOD_H
#define PERIOD_H

#include <QObject>
#include <QDate>

/// Класс представляет рассчетный период
class Period : public QObject
{

    Q_OBJECT

    Q_PROPERTY(QDate from READ from WRITE setFrom NOTIFY fromChanged)
    Q_PROPERTY(QDate to READ to WRITE setTo NOTIFY toChanged)

public:

    explicit Period(QObject *parent = nullptr);
    Period(QDate from, QDate to, QObject * parent = nullptr);

    QDate from() const;
    QDate to() const;


public slots:

    void setFrom(QDate from);
    void setTo(QDate to);


signals:

    void fromChanged(QDate from);
    void toChanged(QDate to);


private:

    QDate _from;
    QDate _to;

};

#endif // PERIOD_H
