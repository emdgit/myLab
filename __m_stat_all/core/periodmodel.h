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

    Q_INVOKABLE
    Period *period(int index) const;

    void setStartPoint(const QDate &date);


protected:

    void reloadPeriods();


private:

    QDate _startPoint;

    std::vector<Period*> _periods;

};

#endif // PERIODMODEL_H
