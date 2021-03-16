#pragma once

#include <QAbstractListModel>

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

        NodeMeta(st_t *st, const index_t &ind, int depth);
        ~NodeMeta();

        st_t *st;
        node_t *parent = nullptr;

        index_t index;
        std::list<node_t*> children;

        struct {
            int depth : 31;
            int expand_flag : 1;    // 1 - expanded
        };

        int rowCount() const;

        void expand() noexcept;
        void collapse() noexcept;
        void addChild(node_t *n);

        bool operator<(const NodeMeta &other) const;
    };

    using NodeList = std::list<NodeMeta*>;
    using NodePtrVec = std::vector<NodeMeta*>;

    Q_OBJECT

public:

    PurchaseGroupModel( QObject * parent = nullptr );
    PurchaseGroupModel( PGStorage *st, QObject * parent = nullptr );
    ~PurchaseGroupModel() override;

    // QAbstractItemModel interface
    int         rowCount(const QModelIndex & parent) const override;
    QVariant    data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE
    QString     groupName(int row) const;

    Q_INVOKABLE
    QString     groupIndex(int row) const;

    Q_INVOKABLE
    bool        hasUnderGroup(int row) const;

    Q_INVOKABLE
    int         depth(int row) const;

    Q_INVOKABLE
    void        expand(int row);

    Q_INVOKABLE
    void        collapse(int row);

    Q_INVOKABLE
    bool        isExpanded(int row) const;

    /// Проверить, есть ли группа с заданным именем в рамках
    /// группы по идексу parentIndex
    Q_INVOKABLE
    bool        hasGroup(const QString &name, const QString &parentIndex) const;

    /// Вернуть идентификатор группы
    Q_INVOKABLE
    int         groupId(int row) const;

    /// Эквивалент 'resetModel()'
    void        reloadData();

    void        init();


protected:

    /// Вернуть NodeMeta, соответствующий данной строке.
    NodeMeta *  node(int row) const;
    NodeMeta *  siblingNode(NodeMeta * n) const;
    void        updateProetcion(NodeMeta * n = nullptr);
    void        clearData();


private:

    PGStorage * _st = nullptr;

    NodeList    _node_list;

    NodePtrVec  _node_proection;
};
