#pragma once

#include <QAbstractListModel>

#include <set>

#include "storagedefinition.h"

/// Плоская интерпретаця иерархических данных
/// о группах. Потому что туго с QML TreeView.
class PurchaseGroupModel : public QAbstractListModel
{

    enum GroupRole {
        Name = Qt::UserRole    /// Имя группы. Роль
    };

    /// Отображение иерархического узла.
    struct NodeMeta {
        using node_t = NodeMeta;
        using index_t = PNodeIndex;
        using st_t = PGStorage;

        NodeMeta(st_t *st, const index_t &ind,
                 int depth, node_t *parent = nullptr);

        st_t *st;
        node_t *parent;

        index_t index;
        std::list<node_t> children;

        struct {
            int depth : 16;
            int rows_under : 15;
            int expand_flag : 1;    // 1 - expanded
        };

        int rowCount() const;
        int expand();
        int collapse();

        bool operator<(const NodeMeta &other) const;
    };

    using NodeList = std::list<NodeMeta>;
    using IndexSet = std::set<PNodeIndex>;

    Q_OBJECT

public:

    PurchaseGroupModel( QObject * parent = nullptr );
    PurchaseGroupModel( PGStorage *st, QObject * parent = nullptr );

    // QAbstractItemModel interface
    int         rowCount(const QModelIndex & parent) const override;
    QVariant    data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE
    QString     groupName(int row) const;

    Q_INVOKABLE
    bool        hasUnderGroup(int row) const;

    Q_INVOKABLE
    int         depth(int row) const;

    Q_INVOKABLE
    void        expand(int row);

    Q_INVOKABLE
    void        collapse(int row);

    /// Зарегистрировать имена ролей в модели           !!! MAYBE RUDIMENTAL !!!
    QHash<int, QByteArray> roleNames() const override;

    /// Вернуть идентификатор группы
    Q_INVOKABLE
    int         groupId( const QModelIndex &index ) const;

    /// Вернуть глубину элемента в дереве
    Q_INVOKABLE
    int         depth(const QModelIndex &index) const;

    /// Эквивалент 'resetModel()'
    void        reloadData();

    void        init();


protected:

    /// Взять PNodeIndex из QModelIndex
    PNodeIndex *toPNodeIndex( const QModelIndex &index ) const noexcept;
    /// Сохранить index в списке используемых, вернуть указатель
    /// на сохраненный экземпляр
    PNodeIndex *insertAndGetRawPtr(PNodeIndex index) const;
    /// Вернуть NodeMeta, соответствующий данной строке.
    NodeMeta *node(int row) const;


private:

    PGStorage * _st = nullptr;

    /// Задействованые объекты PNodeIndex.
    /// Используются как internalPointer внутри QModelIndex
    mutable IndexSet    _index_set;

    NodeList            _node_list;



};
