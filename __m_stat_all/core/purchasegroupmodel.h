#ifndef PURCHASEGROUPMODEL_H
#define PURCHASEGROUPMODEL_H

#include <QAbstractItemModel>

#include <set>

#include "storagedefinition.h"

/// Иерархичная модель отображения групп
class PurchaseGroupModel : public QAbstractItemModel
{

    using IndexSet = std::set<PNodeIndex>;

    enum GroupRole {
        Name = Qt::UserRole    /// Имя группы. Роль
    };

    Q_OBJECT

    Q_PROPERTY(bool showRoot
               READ showRoot
               WRITE setShowRoot
               NOTIFY showRootChanged)

    Q_PROPERTY(QString rootName
               READ rootName
               WRITE setRootName
               NOTIFY rootNameChanged)

public:

    PurchaseGroupModel( QObject * parent = nullptr );
    PurchaseGroupModel( PGStorage *st, QObject * parent = nullptr );

    // QAbstractItemModel interface
    QModelIndex index(int row, int column, const QModelIndex & parent) const override;
    QModelIndex parent(const QModelIndex & child) const override;

    int         rowCount(const QModelIndex & parent) const override;
    int         columnCount(const QModelIndex & parent) const override;

    QVariant    data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE
    int         groupId( const QModelIndex &index ) const;

    bool        showRoot() const;

    /// Эквивалент 'resetModel()'
    void        reloadData();

    QString     rootName() const;


public slots:

    void        setShowRoot(bool showRoot);
    void        setRootName(QString rootName);


signals:

    void        showRootChanged(bool showRoot);
    void        rootNameChanged(QString rootName);


protected:

    PNodeIndex *toPNodeIndex( const QModelIndex &index ) const noexcept;
    PNodeIndex *insertAndGetRawPtr(PNodeIndex index) const;


private:

    PGStorage * _st;

    /// Задействованые объекты PNodeIndex.
    /// Используются как internalPointer внутри QModelIndex
    mutable IndexSet _index_set;

    /// Значение, которое будет отображаться для "мнииого корнегого" элемента,
    /// если флаг '_showRoot' установлен.
    QString     _rootName = "root";

    /// Флаг, обозначающий, следует ли отображать 'мнимый корневой' элемент.
    /// Используется в GUI для добавления новых корневых групп.
    bool        _showRoot = false;

};

#endif // PURCHASEGROUPMODEL_H
