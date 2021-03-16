#include "pnode.h"

#include <regex>

IndexTree::IndexTree() : i( -1 ) {}

IndexTree::IndexTree(int val) : i( val ) {}

void IndexTree::push(PNodeIndex index)
{
    if ( index.indexQueue.empty() )
        return;

    const int v = index.indexQueue.front();

    for ( size_t j(0); j < children.size(); ++j )
    {
        if ( children[j].i == v )
        {
            index.popFront();
            children[j].push( index );
            return;
        }
    }

    children.push_back( IndexTree( v ) );
    index.popFront();
    children.back().push( index );
}

void IndexTree::removeLast(PNodeIndex index)
{
    if ( index.indexQueue.empty() )
        return;

    const int v = index.indexQueue.front();

    std::vector<IndexTree>::iterator it = children.begin();

    for ( ; it != children.end(); ++it )
    {
        if ( (*it).i == v )
        {
            if ( index.indexQueue.size() == 1 )
            {
                children.erase( it );
                return;
            }
            else
            {
                index.popFront();
                (*it).removeLast( index );
            }
        }
    }
}

void IndexTree::indexesAtDistance(PNodeIndex from, PNodeIndex copy,
                                  PIndexVec &to, int distance, int dOrigin) const
{
    if ( from.indexQueue.empty() )  //  Collect
    {
        if ( distance < 0 )
            return;

        PNodeIndex found = PNodeIndex( copy );

        if ( distance < dOrigin )
        {
            found += i;
            to.push_back( PNodeIndex( found ) );
        }

        const size_t s = children.size();

        for ( size_t j(0); j < s; ++j )
            children[j].indexesAtDistance( from, found, to, distance - 1 );
    }
    else    //  continue find
    {
        const int v = from.front();
        const size_t s = children.size();

        from.popFront();
        PNodeIndex next = from;

        for ( size_t j(0); j < s; ++j )
            if ( children[j].i == v )
                children[j].indexesAtDistance( next, copy, to, distance );
    }
}

void IndexTree::clear()
{
    for ( size_t j(0); j < children.size(); ++j )
        children[j].clear();
    children.clear();
}

int IndexTree::distance(PNodeIndex index, int way) const
{
    if ( index.indexQueue.empty() )
        return children.empty() ? 0 : -1;

    const int v = index.indexQueue.front();

    if ( children.empty() )
        return static_cast<int>( index.indexQueue.size() );

    for ( size_t j(0); j < children.size(); ++j )
    {
        if ( children[j].i == v )
        {
            index.popFront();
            return children[j].distance( index, ++way );
        }
    }

    return -1;
}






PNodeIndex::PNodeIndex()
{
}

PNodeIndex::PNodeIndex(const PNodeIndex &other) :
    indexQueue(other.indexQueue)
{
}

PNodeIndex::PNodeIndex(std::initializer_list<int> && vals)
{
    for ( const int &i : vals ) {
        indexQueue.push_back( i );
    }
}

int PNodeIndex::front() const
{
    return indexQueue.front();
}

int PNodeIndex::back() const
{
    return indexQueue.back();
}

void PNodeIndex::popBack()
{
    indexQueue.pop_back();
}

void PNodeIndex::popFront()
{
    indexQueue.pop_front();
}

bool PNodeIndex::isValid() const noexcept
{
    if (indexQueue.size() == 1) {
        return indexQueue.front() >= 0;
    }

    return true;
}

size_t PNodeIndex::size() const noexcept
{
    return indexQueue.size();
}

bool PNodeIndex::isPartOf(const PNodeIndex &other) const
{
    const size_t s = std::min( indexQueue.size(), other.indexQueue.size() );

    for ( size_t i(0); i < s; ++i )
        if ( indexQueue.at( i ) != other.indexQueue.at( i ) )
            return false;

    return true;
}

std::string PNodeIndex::toString() const
{
    if (indexQueue.empty()) {
        return {};
    }

    std::string out_str;

    for (const int &v : indexQueue) {
        out_str += '/' + std::to_string(v);
    }

    return out_str;
}

PNodeIndex PNodeIndex::fromString(const std::string &str)
{
    if (str.empty()) {
        return {};
    }

    if (str.size() == 1) {
        return {-1};
    }

    std::regex full_rx("(\\/(\\d+))+");

    if (!std::regex_match(str, full_rx)) {
        throw std::runtime_error("Invalid input str. Cannot construct PNodeIndex");
    }

    std::regex d_rx("\\/(\\d+)");
    std::sregex_iterator d_begin(str.begin(), str.end(), d_rx);
    std::sregex_iterator d_end = {};

    PNodeIndex out_index;

    for (auto it = d_begin; it != d_end; ++it) {
        out_index += atoi((*it)[1].str().c_str());
    }

    return out_index;
}

bool PNodeIndex::operator==(const PNodeIndex & other) const
{
    if ( indexQueue.size() != other.indexQueue.size() )
        return false;

    const size_t s = indexQueue.size();

    for ( size_t i(0); i < s; ++i )
        if ( indexQueue.at( i ) != other.indexQueue.at( i ) )
            return false;

    return true;
}

bool PNodeIndex::operator<(const PNodeIndex & other) const
{
    if ( indexQueue.size() < other.indexQueue.size() ) {
        return true;
    } else if ( indexQueue.size() > other.indexQueue.size() ) {
        return false;
    }

    auto it_this = indexQueue.cbegin();
    auto it_other = other.indexQueue.cbegin();

    while ( it_this != indexQueue.cend() ) {
        if ( *it_this < *it_other ) {
            return true;
        } else if ( *it_this > *it_other ) {
            return false;
        }

        ++it_this;
        ++it_other;
    }

    return false;
}

PNodeIndex PNodeIndex::operator+(const int & i) const
{
    PNodeIndex index;
    index.indexQueue = indexQueue;
    index.indexQueue.push_back( i );

    return index;
}

PNodeIndex &PNodeIndex::operator+=(const int & i)
{
    indexQueue.push_back( i );

    return *this;
}

PNodeIndex &PNodeIndex::operator=(const PNodeIndex & other)
{
    if ( this == &other )
        return *this;

    indexQueue = other.indexQueue;
    return *this;
}

PNodeIndex::operator bool() const noexcept
{
    return !indexQueue.empty();
}

