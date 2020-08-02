#ifndef PURCHASEMODELDAILY_H
#define PURCHASEMODELDAILY_H

#include <QObject>
#include <QDate>

#include <set>

/*!
 * Класс - модель данных для представления отдельных транзакций
 * в виде блоков, сгруппированных по дням.
 *
 * \details Данные лежат в Storage. Их подгрузка и менеджмент
 * осуществляются средствами CoreAPI, этот класс нужен исключительно
 * для предоставления данных в UI
 */
class PurchaseModelDaily : public QObject
{

    Q_OBJECT

    struct map_node {
        int s_begin, s_end;
        int p_begin, p_end;
    };

    using map_t = std::vector<map_node>;
    using date_set = std::set<QDate>;

public:
    explicit PurchaseModelDaily(QObject *parent = nullptr);

    /*!
     * Вернуть количество разных дней в периоде.
     */
    Q_INVOKABLE
    int     days() const;

    /// Количество транзакций за день
    Q_INVOKABLE
    int     count(int day) const;

    /// Вернуть наименование транзакции
    /*!
     * \param[in] day Порядковый номер дня, начиная с последнего
     * \param[in] row Номер транзакции внутри
     */
    Q_INVOKABLE
    QString name(int day, int row) const;

    /// Составить карту транзакций заново
    void    reloadMap();


protected:

    QDate   startDate() const;
    QDate   finishFate() const;


private:

    map_t       map_;

    date_set    dates_;

};

#endif // PURCHASEMODELDAILY_H
