#ifndef RECORDBYGROUPMODEL_H
#define RECORDBYGROUPMODEL_H

#include <QObject>

class PurchaseRecord;

/// Модель представляет список записей,
/// соответствующих некоторой группе.
class RecordByGroupModel : public QObject
{
    Q_OBJECT

    using record_vec = std::vector<PurchaseRecord*>;

public:
    explicit RecordByGroupModel(QObject *parent = nullptr);

    QStringList model() const;

    Q_INVOKABLE
    void        setGroupId(int id);

    Q_INVOKABLE
    int         rowCount() const noexcept;

    Q_INVOKABLE
    int         groupId() const noexcept;

    Q_INVOKABLE
    QString     recordName(int row) const;


protected:

    /// Узнать, является ли группа прибыльной.
    /*!
     * \exception Кидает исключение, если
     * группа не найдена.
     */
    bool isProfit(int groupId) const;
    PurchaseRecord * record(int row) const;


private:

    /// Список индексов, соответствующих записям
    /// в реальном векторе данных.
    std::vector<int>    record_indexes_;

    /// Кэширование данных о доходных / расходных идентификаторах.
    mutable std::unordered_map<int, bool> profit_cache_;

    /// Все доходные группы.
    record_vec *        records_profit_;
    /// Все расходные группы.
    record_vec *        records_spend_;

    /// Идентификатор текущей группы.
    int                 group_id_ = -1;

    /// Флаг с каким списком работаем.
    bool                is_profit_ = false;

};

#endif // RECORDBYGROUPMODEL_H
