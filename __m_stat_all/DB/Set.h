#pragma once

#include <iostream>
#include <memory>

typedef unsigned int uint;

template <typename T>
struct CompareBase
{
	static inline bool less(const T &v1, const T &v2) { return (v1 < v2); }
	static inline bool equal(const T &v1, const T &v2) { return (v1 == v2); }
};

template <typename T>
struct ComparePtr : CompareBase<std::shared_ptr< T >>
{
	typedef typename std::shared_ptr< T >	_Ptr;
	static inline bool less(const _Ptr &v1, const _Ptr &v2) { return (*v1.get() < *v2.get()); }
	static inline bool equal(const _Ptr &v1, const _Ptr &v2) { return (*v1.get() == *v2.get()); }
};

template <typename T, typename Compare = CompareBase<T>>
struct AdderBase
{
	using _comp = Compare;

	/*!
	 * \brief add
	 * \param _counter
	 * \param _to
	 * \param _from
	 * \return  true - if need continue after doing this func
	 */
	static inline bool add(uint &_counter, const T &_to, const T &_from) noexcept
	{
		(void)_counter;
		if (_comp::equal(_to, _from))
			return false;
		return true;
	}
};

template <typename T, typename Compare = CompareBase<T>>
struct AdderMulti : AdderBase<T, Compare>
{
	using _comp = Compare;

	static inline bool add(uint &_counter, const T &_to, const T &_from) noexcept
	{
		if (_comp::equal(_to, _from))
		{
			++_counter;
			return false;
		}
		return true;
	}
};

namespace Owl
{
	template <	typename T,
				typename Compare = CompareBase<T>,
				typename Adder = AdderBase<T, Compare> >
	class Set;


	template <	typename T,
				typename D = std::shared_ptr<T>,
				typename Compare = ComparePtr<T> >
	class PSet;


	template <	typename T,
				typename Compare = CompareBase<T>,
				typename Add = AdderMulti<T, Compare> >
	class MSet;


	template <	typename T,
				typename D = std::shared_ptr<T>,
				typename Compare = ComparePtr<T>,
				typename Add = AdderMulti<D, Compare> >
	class MPSet;
}

template <typename T,
	typename Compare = CompareBase<T>,
	typename Adder = AdderBase<T, Compare>>
	struct Tree
{

	using _comp = Compare;
	using _adder = Adder;
	using Node = Tree<T, _comp, _adder>;

	Tree() : left(nullptr), right(nullptr), parent(nullptr) {}
	Tree(const T &val) : data(val), left(nullptr), right(nullptr), parent(nullptr) {}
	Tree(const T &val, Node * const &par) : data(val), left(nullptr), right(nullptr), parent(par) {}
	~Tree() {}

	bool            push(const T &val) noexcept
	{
		if (!_adder::add(count, data, val))
			return false;

		auto tree = _comp::less(val, data) ? &left : &right;

		if (*tree)
			return (*tree)->push(val);
		else
			*tree = new Node(val, this);

		return true;
	}

	Node *          min() const noexcept
	{
		if (!left)
			return const_cast<Node*>(this);

		return left->min();
	}
	Node *          max() const noexcept
	{
		if (!right)
			return const_cast<Node*>(this);

		return right->max();
	}
	Node *          find(const T &val) const noexcept
	{
		if (_comp::equal(val, data))
			return const_cast<Node*>(this);

		auto node = _comp::less(val, data) ? left : right;

		if (!node)
			return nullptr;

		return node->find(val);
	}

	void			clear() noexcept
	{
		if (left)
		{
			left->clear();
			delete left;
		}

		if (right)
		{
			right->clear();
			delete right;
		}
	}


	T               data;

	uint            count = 1;

	Node *          left;
	Node *          right;
	Node *          parent;
};


template < typename T, typename Compare, typename Adder>
	class Owl::Set
{
public:
	Set() : _root(nullptr), _size(0) { _end = new _Tree(); }
	~Set() {}

	using _Tree = Tree<T, Compare, Adder>;


	class Iterator
	{

	public:
		Iterator() = delete;
		Iterator(_Tree * const &node) : _node(node) {}

		T &				operator*()
		{
			return _node->data;
		}

		Iterator &		operator++()
		{
			if (_node->count > _index)
			{
				++_index;
				return *this;
			}
			if (_node->right)
			{
				_node = _node->right->min();
				_index = 1;
				return *this;
			}

			_stepUp();

			return *this;
		}
		Iterator &		operator--()
		{
			if (_index > 1)
			{
				--_index;
				return *this;
			}
			if (_node->left)
			{
				_node = _node->left->max();
				_index = _node->count;
				return *this;
			}

			_stepDown();
			return *this;
		}

		Iterator		operator++(int)
		{
			auto it = Iterator(_node);
			++(*this);
			return it;
		}
		Iterator		operator--(int)
		{
			auto it = Iterator(_node);
			--(*this);
			return it;
		}

		bool			operator==(const Iterator &other)
		{
			return ((_node == other._node)&&(_index == other._index));
		}
		bool			operator!=(const Iterator &other)
		{
			return !(*this == other);
		}


	protected:

		void			_stepUp() noexcept
		{
			auto parent = _node->parent;
			_index = 1;

			if (!parent)
			{
				_node = parent;
				return;
			}

			if (_node == parent->left)
			{
				_node = parent;
				return;
			}

			_node = parent;
			_stepUp();
		}
		void			_stepDown() noexcept
		{
			auto parent = _node->parent;

			if (!parent)
			{
				_node = parent;
				return;
			}

			if (_node == parent->right)
			{
				_node = parent;
				_index = _node->count;
				return;
			}

			_node = parent;
			_index = _node->count;
			_stepDown();
		}


	private:

		_Tree *			_node;

		uint            _index = 1;

	};


	inline const uint &	size() const noexcept { return _size; }

	inline bool			isEmpty() const noexcept { return _size == 0; }

    Iterator			begin() const noexcept
	{
		if (!_root)
			return Iterator(_end);

		return Iterator(_root->min());
	}
    inline Iterator		end() const noexcept { return Iterator(_end); }
	Iterator			find(const T &val) const noexcept
	{
		if (!_root)
			return end();

		return Iterator(_root->find(val));
	}

	void				insert(const T &val) noexcept
	{
		if (!_root)
		{
			_root = new _Tree(val);
			_root->parent = _end;
			_end->left = _root;
			_end->right = _root;
			++_size;
			return;
		}

		if (_root->push(val))
			++_size;
	}
	void				erase(const T &val) noexcept
	{
		auto node = _root->find(val);

		if (!node)
			return;

		auto underRight = node->right;
		auto underLeft = node->left;

		if (node == _root)
		{
			delete _root;

			if (underRight)
			{
				_root = underRight;
				_root->parent = nullptr;

				auto min = _root->min();
				min->left = underLeft;
				underLeft->parent = min;
			}
			else if (underLeft)
			{
				_root = underLeft;
				_root->parent = nullptr;
			}
			else
			{
				_root = nullptr;
				_end->left = nullptr;
				_end->right = nullptr;
			}

			--_size;
			return;
		}

		auto parent = node->parent;
		auto link = parent->left == node ? &parent->left : &parent->right;

		delete node;

		if (underRight)
		{
			*link = underRight;
			underRight->parent = parent;

			auto min = underRight->min();
			min->left = underLeft;
		}
		else if (underLeft)
		{
			*link = underLeft;
			underLeft->parent = parent;
		}
		else
		{
			*link = nullptr;
		}

		--_size;
	}
	void				clear() noexcept
	{
		if (!_root)
			return;

		_root->clear();
		delete _root;
		_root = nullptr;
		_size = 0;
	}


protected:

	_Tree *				_end;
	_Tree *             _root;

	uint				_size;
};


template <typename T, typename D, typename Compare>
class Owl::PSet : public Owl::Set<D, Compare>
{};

template <typename T, typename Compare, typename Add>
class Owl::MSet : public Owl::Set<T, Compare, Add>
{};

template <typename T, typename D,
	typename Compare, typename Add >
	class Owl::MPSet : public Owl::Set<D, Compare, Add>
{};

