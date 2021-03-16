#ifndef PNODE_H
#define PNODE_H

#include <QVariant>
#include <QDebug>

#include <queue>

/// Структура представляет собой адрес элемента в дереве.
/*!
 * \brief   struct-address in tree
 *
 * \details PNodeIndex is a structure that represents
 *          address in a graph.
 *          You can use it in Qt's signal-slot system.
 *
 * \example ( 0/1/2/3 ) - address of the fourth element
 *          according to his parent, who is third ( 0/1/2 ),
 *          who is second ( 0/1 ) who is first ( 0 ).
 *          Counting starts with 0.
 */
struct PNodeIndex
{
    PNodeIndex();
    PNodeIndex( const PNodeIndex &other );
    PNodeIndex( std::initializer_list<int> &&vals );
    PNodeIndex(PNodeIndex&&) = default;

    std::deque<int> indexQueue;

    /// Самое левое значение (первое)
    int             front() const;
    /// Самое правое значение (последнее)
    int             back() const;

    /// Удалить последний индекс
    void            popBack();
    /// Удалить первый индекс
    void            popFront();

    /// Проверить, является ли индекс валидным.
    /// Невалидный в случае, если содержит 1 отрицательный элемент.
    bool            isValid() const noexcept;

    size_t          size() const noexcept;

    /// Проверяет, является ли данный индекс частью переданного
    bool            isPartOf( const PNodeIndex &other ) const;

    /// Сериализация в строку типа '/x/y/k'
    /*!
     * \example
     * Индекс {0, 2, 4} -> "/0/2/4"
     * Индекс {2} -> "/2"
     * Индекс {} -> ""
     */
    std::string     toString() const;

    /// Конструирует Индекс по переданной строке.
    /*!
     * \param[in] str Строка типа '/x/y/k', или пустая строка.
     * Строка длины 1 - невалидный индекс.
     * \throw runtime_error в случае некорректной строки.
     */
    static PNodeIndex fromString(const std::string &str);

    /// Сериализация вывода в qDebug()
    friend QDebug & operator<<( QDebug &out, const PNodeIndex &ind )
    {
        std::deque<int> d = ind.indexQueue;

        QString str = "PNodeIndex ( ";

        while ( !d.empty() )
        {
            str += QString::number( d.front() );
            str += d.size() == 1 ? "" : "/";
            d.pop_front();
        }

        str += " )";

        out << str;
        return out;
    }

    bool            operator==( const PNodeIndex &other ) const;
    bool            operator<( const PNodeIndex &other ) const;

    /*!
     * \brief Добавляет число в конец (справа).
     *        Возвращает модифицированную копию
     */
    PNodeIndex      operator+( const int &i ) const;

    /// Добавляет число в конец
    PNodeIndex &    operator+=( const int &i );

    PNodeIndex &    operator=( const PNodeIndex &other );


    /// Проверка на пустоту
    operator bool() const noexcept;
};

typedef std::vector<PNodeIndex> PIndexVec;

Q_DECLARE_METATYPE(PNodeIndex)



/*!
 * \brief   The IndexTree struct
 *
 * \details A service struct that represents amount of tree indexes.
 *          It works with PNodeIndex and merges them by calling push().
 *
 * \example ( 0/2/2/3 ) & ( 0/2/4/4 ) & ( 0/2/4/5/6 )
 *
 *          ===========     Three given in an example PNodeIndex'es
 *          =   0     =     together will build that structure.
 *          =   |     =
 *          =   2     =
 *          =  / \    =
 *          = 2   4   =
 *          = |  / \  =
 *          = 3 4   5 =
 *          =       | =
 *          =       6 =
 *          ===========
 */
struct IndexTree
{
    IndexTree();
    IndexTree( int val );

    void push( PNodeIndex index );
    void removeLast( PNodeIndex index );
    void indexesAtDistance( PNodeIndex from, PNodeIndex copy, PIndexVec &to,
                            int distance = 2, int dOrigin = 2 ) const;
    void clear();

    int distance( PNodeIndex index, int way = 0 ) const;

    int i;
    std::vector<IndexTree> children;
};

#endif // PNODE_H
