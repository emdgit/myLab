#pragma once

#include <iostream>
#include <memory>

typedef unsigned int uint;

template <typename T>
struct CompareBase
{
	static inline bool less(const T &v1, const T &v2) { return ( v1 < v2 ); }
	static inline bool equal(const T &v1, const T &v2) { return (v1 == v2); }
};

template <typename T>
struct ComparePtr : CompareBase<T>
{
	typedef typename std::shared_ptr< T >	_Ptr;
	static inline bool less(const _Ptr &v1, const _Ptr &v2) { return (*v1.get() < *v2.get()); }
	static inline bool equal(const _Ptr &v1, const _Ptr &v2) { return (*v1.get() == *v2.get()); }
};

template <typename T, typename Compare = CompareBase<T>>
struct TreeBase
{
	using _comp = Compare;

	TreeBase() = delete;
	TreeBase(const T &val) : data(val), left(nullptr), right(nullptr), parent(nullptr) {}
	TreeBase(const T &val, TreeBase<T> * const &par) : data(val), left(nullptr), right(nullptr), parent(par) {}
	virtual ~TreeBase() { std::cout << "~TreeBase(" << data << ")" << std::endl; }

	bool				push(const T &val) noexcept
	{
		if ( _comp::equal( val, data ) )
			return false;

		auto tree = _comp::less( val , data ) ? &left : &right;

		if (*tree)
			return (*tree)->push(val);
		else
			*tree = new TreeBase<T>(val, this);

		return true;
	}

	TreeBase<T> *		min() const noexcept
	{
		if (!left)
			return const_cast< TreeBase<T>* >(this);

		return left->min();
	}
	TreeBase<T> *		max() const noexcept
	{
		if (!right)
			return const_cast<TreeBase<T>*>(this);

		return right->max();
	}
	TreeBase<T> *		find(const T &val) const noexcept
	{
		if ( _comp::equal( val, data ) )
			return const_cast<TreeBase<T>*>(this);

		auto node = _comp::less(val, data) ? left : right;

		if (!node)
			return nullptr;

		return node->find(val);
	}

	void				clear() noexcept
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


	T					data;

	TreeBase<T> *		left;
	TreeBase<T> *		right;
	TreeBase<T> *		parent;
};

template <typename T, typename Compare = ComparePtr<T>>
struct PtrTree : TreeBase<T, Compare>
{
	PtrTree() = delete;
	PtrTree(const T &val) : TreeBase<T, ComparePtr<T>>( val ) {}
	PtrTree(const T &val, TreeBase<T> * const &par) : TreeBase<T, ComparePtr<T>>(val, par) {}
	~PtrTree() { std::cout << "~Tree(" << data << ")" << std::endl; }

};

template <typename T, typename TreeType = TreeBase<T>>
class Set
{
public:
	Set() : _root(nullptr), _size(0) {}
	~Set() {}


	class Iterator
	{

		using Node = TreeType;

	public:
		Iterator() = delete;
		Iterator(Node * const &node) : _node(node) {}

		T &				operator*()
		{
			return _node->data;
		}

		Iterator &		operator++()
		{
			if (_node->right)
			{
				_node = _node->right->min();
				return *this;
			}

			_stepUp();

			return *this;
		}
		Iterator &		operator--()
		{
			if (_node->left)
			{
				_node = _node->left->max();
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
			return _node == other._node;
		}
		bool			operator!=(const Iterator &other)
		{
			return !(*this == other);
		}


	protected:

		void			_stepUp() noexcept
		{
			auto parent = _node->parent;

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
				return;
			}

			_node = parent;
			_stepDown();
		}


	private:

		Node *			_node;

	};


	inline const uint &	size() const noexcept { return _size; }

	inline bool			isEmpty() const noexcept { return _size == 0; }

	Iterator			begin() noexcept
	{
		if (!_root)
			return Iterator(_root);

		return Iterator(_root->min());
	}
	Iterator			end() noexcept
	{
		return Iterator(nullptr);
	}
	Iterator			find(const T &val) const noexcept
	{
		if (!_root)
			return end();

		return Iterator(_root->find(val));
	}

	void				insert( const T &val ) noexcept
	{
		if (!_root)
		{
			_root = new TreeType(val);
			++_size;
			return;
		}

		if (_root->push(val))
			++_size;
	}
	void				erase( const T &val ) noexcept
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

	TreeType *			_root;

	uint				_size;
};


template <typename T, typename TreeType = PtrTree<T>>
class PtrSet : public Set<T, TreeType>
{};

