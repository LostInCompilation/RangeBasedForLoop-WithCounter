/*
 
 Range-Based for loop with counter variable
 
 ***********************************************************************************
 The zlib License

 Copyright ©2023 Marc Schöndorf

 This software is provided 'as-is', without any express or implied warranty. In
 no event will the authors be held liable for any damages arising from the use of
 this software.

 Permission is granted to anyone to use this software for any purpose, including
 commercial applications, and to alter it and redistribute it freely, subject to
 the following restrictions:

 1.  The origin of this software must not be misrepresented; you must not claim
     that you wrote the original software. If you use this software in a product,
     an acknowledgment in the product documentation would be appreciated but is
     not required.

 2.  Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.

 3.  This notice may not be removed or altered from any source distribution.
 ***********************************************************************************
 
*/

#ifndef RANGE_FOR_LOOP_WITH_COUNTER_H
#define RANGE_FOR_LOOP_WITH_COUNTER_H

#include <iterator>

template<typename IteratorType>
class IteratorCounter
{
private:
    using IndexType = typename std::iterator_traits<IteratorType>::difference_type;
    using IteratorReferenceType = typename std::iterator_traits<IteratorType>::reference;
    
    IteratorType   iterator;
    IndexType      counter;
    
public:
    IteratorCounter() = delete;
    
    explicit IteratorCounter(IteratorType it, IndexType offset = 0)
        : iterator(it)
        , counter(offset)
    {}
    
    IteratorCounter& operator++()
    {
        iterator++;
        counter++;
        
        return *this;
    }
    
    std::pair<IteratorReferenceType, const IndexType&> operator*() const { return {*iterator, counter}; }
    
    bool operator==(const IteratorCounter& rhs) const { return this->iterator == rhs.iterator; }
    bool operator!=(const IteratorCounter& rhs) const { return this->iterator != rhs.iterator; }
};

// For l-values, non owning
template<typename IteratorType>
class IteratorCounterRange_lval
{
private:
    using Iterator = IteratorCounter<IteratorType>;
    using IndexType = typename std::iterator_traits<IteratorType>::difference_type;
    
    IteratorType first;
    IteratorType last;
    IndexType    offset;
    
public:
    IteratorCounterRange_lval() = delete;
    
    explicit IteratorCounterRange_lval(IteratorType begin, IteratorType end, IndexType offset = 0)
        : first(begin)
        , last(end)
        , offset(offset)
    {}
    
    Iterator begin() const { return Iterator(first, offset); }
    Iterator end() const { return Iterator(last, offset); }
};

// For r-values, needs to be owning
template<typename ContainerType, typename IteratorType>
class IteratorCounterRange_rval
{
    
};

//*******************************************************************************
// C-style array
template<typename T, std::size_t size>
decltype(auto) count(T (&arr)[size], std::ptrdiff_t offset = 0)
{
    return IteratorCounterRange_lval(arr, arr + size, offset);
}

//*******************************************************************************
// Range
template<typename IteratorType>
decltype(auto) count(IteratorType first, IteratorType last, typename std::iterator_traits<IteratorType>::difference_type offset = 0)
{
    return IteratorCounterRange_lval(first, last, offset);
}

//*******************************************************************************
// L-Value container
template<typename ContainerType>
decltype(auto) count(ContainerType& container, typename std::iterator_traits<typename ContainerType::iterator>::difference_type offset = 0)
{
    return IteratorCounterRange_lval(std::begin(container), std::end(container), offset);
}

//*******************************************************************************
// R-Value container
template<typename ContainerType>
decltype(auto) count(ContainerType&& container, typename std::iterator_traits<typename ContainerType::iterator>::difference_type offset = 0)
{
    return 0;
}

#endif
