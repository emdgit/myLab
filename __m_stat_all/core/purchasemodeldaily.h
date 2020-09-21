#ifndef PURCHASEMODELDAILY_H
#define PURCHASEMODELDAILY_H

#include <QObject>
#include <QDate>

#include <set>

class Purchase;

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

    /*!
     * \brief Блок покупок. В нем лежит сумма
     * покупок за день по конкретной грппе
     */
    struct day_block {
        double summ = 0.0;
        std::string name;
        std::vector<Purchase*> block_data;
        bool is_profit = false;

        day_block() = delete;
        day_block(const std::string &name, bool is_profit);

        void add(Purchase *p);

        // Является ли блок суммой нескольких покупок
        bool is_complex() const noexcept;
    };

    /*!
     * \brief Данные покупок за день.
     */
    struct day_info {
        std::vector<day_block*> data_;

        QDate date;

        day_info() = delete;
        day_info(const QDate &d);
        ~day_info();

        // Добавить информацию о покупке в список.
        void add(Purchase *p, bool is_profit);
    };

    using day_vec = std::vector<day_info*>;

public:
    explicit PurchaseModelDaily(QObject *parent = nullptr);
    ~PurchaseModelDaily() override;

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

    /// Вернуть сумму транзакции
    /*!
     * \param[in] day Порядковый номер дня, начиная с последнего
     * \param[in] row Номер транзакции внутри
     * \return Сумма транзакции в строковом виде
     */
    Q_INVOKABLE
    QString summ(int day, int row) const;

    /// Вернуть дату транзакции
    /*!
     * \param[in] day Порядковый номер дня, начиная с последнего
     * \return Дата транзакции в строковом виде
     */
    Q_INVOKABLE
    QString date(int day) const;

    /// Является ли транзакция доходной или нет
    /*!
     * \param day Порядковый номер дня, начиная с последнего
     * \param row Номер транзакции внутри
     */
    Q_INVOKABLE
    bool isProfit(int day, int row) const;

    /// Составить карту транзакций заново
    void    reloadMap();


protected:

    day_info * dayInfo(int day) const;

    day_block * dayBlock(int day, int row) const;

    void    clear();


private:

    day_vec     days_;

};

#endif // PURCHASEMODELDAILY_H
