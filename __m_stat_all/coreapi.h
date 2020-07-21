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
class ModelManager;

class CoreAPI : public QObject
{

    Q_OBJECT

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


    static void setModelManager( ModelManager * mm );


    /// Задать хранилище групп дохода
    static void setProfitGroupSt( PGStorage *st ) noexcept;


    /// Задать хранилище групп расхода
    static void setSpendGroupSt( PGStorage *st ) noexcept;


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


private:

    static pg::Worker * _pg_worker;

    static inline ModelManager * _modelManager = nullptr;

    /// Хранилка групп доходов.
    static inline PGStorage *  _p_g_storage_profit;

    /// Хранилка групп попупок.
    static inline PGStorage *  _p_g_storage_spend;

    /// Хранилка доходных записей.
    static inline std::vector<PurchaseRecord*> _records_profit;

    /// Хранилка расходных записей.
    static inline std::vector<PurchaseRecord*> _records_spend;

    /// Идентификатор залогиненого пользователя
    static inline int _current_user = 0;

    /// Идентификатор залогиненой группы пользователей
    static inline int _current_group = 0;

    /// Даты начала рассчетного периода
    static inline QDate _start_point;

};

#endif // COREAPI_H
