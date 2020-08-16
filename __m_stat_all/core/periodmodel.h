#ifndef PERIODMODEL_H
#define PERIODMODEL_H

#include <QObject>
#include <QDate>

#include <vector>

class Period;

class PeriodModel : public QObject
{

    Q_OBJECT

public:
    explicit PeriodModel(QObject *parent = nullptr);

    Q_INVOKABLE
    int size() const;

    /// Вернуть период под данным номером.
    Q_INVOKABLE
    Period *period(int index) const;

    /// Вернуть период, которому принадлежит данная дата
    Period *period(const QDate &date) const;

    Q_INVOKABLE
    QString periodString(int index) const;

    Q_INVOKABLE
    QString dateFrom(int index) const;

    Q_INVOKABLE
    QString dateTo(int index) const;

    void setStartPoint(const QDate &date);


protected:

    void reloadPeriods();


private:

    QDate _startPoint;

    std::vector<Period*> _periods;

};

#endif // PERIODMODEL_H
