#ifndef PURCHASEMODELDAILY_H
#define PURCHASEMODELDAILY_H

#include <QObject>

class PurchaseModelDaily : public QObject
{

    Q_OBJECT

    struct map_node {
        int s_begin = 0, s_end = 0;
        int p_begin = 0, p_end = 0;
    };

    using map_t = std::vector<map_node>;

public:
    explicit PurchaseModelDaily(QObject *parent = nullptr);

    Q_INVOKABLE
    int     days() const; // разные

    Q_INVOKABLE
    int     count(int day) const;

    Q_INVOKABLE
    QString name(int day, int row) const;

    void    reloadMap();


protected:

    QDate   startDate() const;
    QDate   finishFate() const;


private:

    map_t   map_;

};

#endif // PURCHASEMODELDAILY_H
