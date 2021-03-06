#ifndef COREAPI_H
#define COREAPI_H

#include <QObject>
#include <QDate>

#include <vector>

#include "iobject.h"
#include "storagedefinition.h"

namespace pg {
    class Worker;
    class Answer;
}

class PurchaseRecord;
class SignalManager;
class ChartManager;
class ModelManager;
class Purchase;
class Period;

class CoreAPI : public QObject
{

    Q_OBJECT

    using date_pair = std::pair<QDate, QDate>;

public:

    /*!
     * Добавить запись о покупке.
     * \param[in] rec       Наименование записи
     * \param[in] summ      Потраченная сумма
     * \param[in] date      Дата совершения покупки
     * \param[in] amount    Количество единиц
     */
    Q_INVOKABLE
    static void addPurchase( const QString &rec, QString summ,
                             const QString &date_str, int amount );


    /*!
     * Добавить запись о прибыли.
     * \param[in] rec       Наименование записи
     * \param[in] summ      Полученная сумма
     * \param[in] date      Дата получения суммы
     * \param[in] amount    Количество единиц
     */
    Q_INVOKABLE
    static void addProfit( const QString &rec, QString summ,
                           const QString &date_str, int amount );


    /*!
     * Загрузить корневые группы.
     * В зависимости от переданного параметра profit,
     * загрузка будет либо в хранилище групп доходов,
     * либо групп расходов.
     * \param[in] profit  true - доходы; false - расходы
     */
    static void loadGroups(bool profit = false);


    /*!
     * Загрузить все записи. Конечные единицы товаров.
     * В зависимости от переданного параметра profit,
     * загрузка будет либо в хранилище записей доходов,
     * либо записей расходов.
     * \param[in] profit  true - доходы; false - расходы
     */
    Q_INVOKABLE
    static void loadRecords( bool profit = false );


    Q_INVOKABLE
    static void switchHintModel( bool profit );


    /*!
     * Добавить запись в БД
     * \param[in] groupId Идентификатор группы, в которую будет включена запись
     * \param[in] recordName Имя добавляемой записи
     */
    Q_INVOKABLE
    static void addRecord( int groupId, const QString &recordName, bool profit );


    /*!
     * Добавить новую группу.
     * \param[in] name  Имя добавляемой группы.
     * \param[in] parentGroupId Идетификатор группы родителя, если передать 0,
     * будет создана новая корневая группа.
     * \param[in] profit Расход/доход
     */
    Q_INVOKABLE
    static void addPurchaseGroup( const QString &name, int parentGroupId, bool profit );


    /*!
     * Задать текущего пользователя и его группу.
     * \param uid[in] Идентификатор пользователя.
     * \param guid[in] Идентификатор группы пользователей.
     */
    Q_INVOKABLE
    static void setCurrentUser( int uid, int guid );


    /*!
     * Задано ли начало рассчетного периода.
     */
    Q_INVOKABLE
    static bool hasStartPoint();


    /*!
     * Изменить/задать дату начала рассчетного периода.
     * \warning Не будет работать, если не задан пользователь.
     */
    Q_INVOKABLE
    static void setStartPoint( const QDate &date );


    /*!
     * Проверить переданную строку на валидность даты.
     * \param[in] date_str Дата в виде строки.
     */
    Q_INVOKABLE
    static bool checkDateFormat( const QString &date_str );


    /// Проверить валидность введенных данных в поле суммы.
    Q_INVOKABLE
    static bool checkSummStr(const QString &summ_str);


    /*!
     * Вернуть значение расходов за текущий период.
     */
    Q_INVOKABLE
    static double currentConsuption();


    /*!
     * Вернуть значение доходов за текущий период.
     */
    Q_INVOKABLE
    static double currentProfit();


    /*!
     * \brief Загрузить информацию о покупках за период.
     * Общие суммы по конкретным группам.
     * \param[in] dateFrom Дата "с" в строковом виде
     * \param[in] dateTo Дата "по" в строковом виде
     */
    Q_INVOKABLE
    static void loadPurchasesSumm(const QString &dateFrom,
                                  const QString &dateTo,
                                  bool profit);


    /*!
     * \brief Загрузить информацию о покупках за текущий период
     */
    Q_INVOKABLE
    static void loadPurchasesSumm(bool profit);


    /*!
     * \brief loadPurchases Загрузить покупки за период
     * \param[in] dateFrom Дата с
     * \param[in] dateTo Дата по
     * \param[in] profit Доход/расход
     */
    Q_INVOKABLE
    static void loadPurchases(const QString &dateFrom,
                              const QString &dateTo,
                              bool profit);


    /*!
     * \brief Загрузить покупки за текущий период
     * \param[in] profit Доход/расход
     */
    Q_INVOKABLE
    static void loadPurchases(bool profit);


    /*!
     * \brief   Установить период, который показывается в
     *          GUI PurchaseView
     * \param[in] period Номер периода
     */
    Q_INVOKABLE
    static void setPurchaseViewPeriod(int period);


    /*!
     * \brief Вернуть чистую прибыль за все время
     */
    Q_INVOKABLE
    static int  getCleanProfit();


    /*!
     * \brief Вернуть сохраненный процент за все время
     */
    Q_INVOKABLE
    static QString getCleanPercent();


    /*!
     * \brief Загрузить данные для главного графика (прибыль / расход)
     */
    Q_INVOKABLE
    static void updateMainChartData();


    static void setModelManager(ModelManager * mm) noexcept;
    static void setSignalManager(SignalManager * sm) noexcept;
    static void setChartManager(ChartManager *cm) noexcept;


    /// Задать callback групп дохода
    static void initProfitGroupCallback() noexcept;


    /// Задать callback групп расхода
    static void initSpendGroupCallback() noexcept;


signals:

    void purchaseAdded();


protected:

    /// Загрузить группы для данного 'parent', упаковать их в 'st'
    static void loadGroupsByParent( PNodeIndex parent, PGStorage * st );

    /// Разобрать группы из answer и уложить в хранилище под индекс parent
    static void packGroups( pg::Answer * answer, PGStorage * st, PNodeIndex parent );

    /// Загрузить начало рассчетного периода. При смене и установке группы польователей.
    static void reloadStartPoint();

    /// Попытаться конвертировать date_str в объект даты.
    static bool dateFromStr( const QString &date_str, QDate &date );

    /// Добавить информацию о покупке / прибыли.
    static void addTransaction( const QString &rec, QString summ,
                                const QString &date_str, int amount, bool profit );

    /// Вернуть даты начала и конца текущего периода.
    static std::pair<QDate,QDate> currentPeriod();

    /// Вернуть корневые группы для записей переданного периода.
    /*!
     * \param[in] dpair Даты конца и начала периода.
     * \param[in] profit Прибыль/траты
     * \return Вектор идентификаторов корневых групп
     */
    static std::vector<int> periodRootGroups(date_pair dpair, bool profit );

    /// Посчитать сумму всех записей за текущий период.
    static double currentPeriodSumm(bool profit);

    /// Загрузить информацию о покупках за период.
    static void loadPurchasesSumm(const QDate &from, const QDate &to, bool profit);

    /// Загрузить информацию о покупках за период.
    static void loadPurchases(const QDate &from, const QDate &to, bool profit);


private:

    static pg::Worker * _pg_worker;

    static inline ModelManager * _modelManager = nullptr;

    static inline SignalManager * _signalManager = nullptr;

    static inline ChartManager * _chartManager = nullptr;

    /// Идентификатор залогиненого пользователя
    static inline int _current_user = 0;

    /// Идентификатор залогиненой группы пользователей
    static inline int _current_group = 0;

    /// Даты начала рассчетного периода
    static inline QDate _start_point;

    /// Период, который показывается в GUI PurchaseView
    static inline int _purchase_view_period = -1;

};

#endif // COREAPI_H
