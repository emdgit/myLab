#ifndef HIERARCHICALSTORAGE_H
#define HIERARCHICALSTORAGE_H

#include <vector>
#include <stdexcept>
#include <functional>

#include "pnode.h"

/// todo comment ( < required )
/*!
 * \brief           Структура представляет узел дерева.
 *
 * \tparam Object   Тип хранимого объекта.
 * \tparam Key      Тип ключа.
 * \tparam GetKey   Указатель на функцию получения ключа. Ключ -
 *                  значение, по которому определяется уникальность объекта.
 */
template < class Object, class Key, class GetKey = Key(Object::*)()const >
struct HNode
{
    using node_type = HNode<Object,Key,GetKey>;

    HNode( Object *data ) : _data(data) {}
    HNode( Object *data, node_type *parent ) :
        _data(data), _parent(parent) {}

    ~HNode() {
        for ( auto &c : _children )
            delete c;
        _children.clear();
        delete _data;
    }

    Object *    _data = nullptr;

    node_type * _parent = nullptr;

    std::vector<node_type*> _children;

    bool operator<(const node_type &node) const {
        if ( !_data || !node._data ) {
            throw std::runtime_error( "Attempt to compare(less) invalid HNodes" );
        }

        return *_data < *node._data;
    }

    bool operator==(const node_type &node) const {
        if ( !_data || !node._data ) {
            throw std::runtime_error( "Attempt to compare invalid HNodes" );
        }

        return (!(*_data < *node._data) && !(*node._data < *_data));
    }
};

// todo clear() remove(PNI)
template < class Object, class Key, Key(Object::*get_key_f)()const >
class HierarchicalStorage
{

    using GetKey = Key(Object::*)()const;
    using node_type = HNode<Object, Key, GetKey>;
    using callback_func = std::function<void(PNodeIndex)>;

public:

    HierarchicalStorage(){}

    /// Установить callback, оповещающий о вставке.
    /*!
     * Если обработчик установлен, он будет вызван с
     * входным параметром, равным индексу нового элемента.
     */
    void    setInsertHandler( callback_func f )
    {
        _on_inserted_f = f;
    }

    void    insert( const PNodeIndex &parent, Object *obj )
    {
        if ( !parent ) {
            _roots.push_back( new node_type( obj, nullptr ) );
            if ( _on_inserted_f ) {
                _on_inserted_f( {static_cast<int>( _roots.size() - 1 )} );
            }
            return;
        }

        auto parent_node = node( parent );

        if ( !parent_node ) {
            throw std::runtime_error( "Unexistend parent index!" );
        }

        parent_node->_children.push_back( new node_type( obj, parent_node ) );

        if ( _on_inserted_f ) {
            int last_index = parent_node->_children.size() - 1;
            _on_inserted_f( parent + last_index );
        }
    }

    /// Удалить все записи
    void    clear()
    {
        for ( auto &r : _roots ) {
            delete r;
        }

        _roots.clear();
    }

    node_type * node( PNodeIndex index ) const noexcept
    {
        if ( !index ) {
            return nullptr;
        }

        if ( index.front() >= static_cast<int>(_roots.size()) ) {
            return nullptr;
        }

        node_type * out = _roots[index.front()];
        index.popFront();

        while ( index ) {
            auto n = index.front();

            if ( n >= static_cast<int>(out->_children.size()) ) {
                /// Нет такого элемента
                return nullptr;
            }

            out = out->_children[n];
            index.popFront();
        }

        return out;
    }

    int childCount( PNodeIndex index ) const noexcept
    {
        // if is empty
        if ( !index ) {
            return _roots.size();
        }

        auto _node = node( index );

        if ( !_node ) {
            return 0;
        }

        return _node->_children.size();
    }

    callback_func insertHandler() const noexcept
    {
        return _on_inserted_f;
    }


private:

    std::vector<node_type*> _roots;

    callback_func _on_inserted_f = nullptr;

};

#endif // HIERARCHICALSTORAGE_H
