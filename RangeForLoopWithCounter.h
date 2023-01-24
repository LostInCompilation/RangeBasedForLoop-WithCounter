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
    
    explicit IteratorCounter(const IteratorType& it, const IndexType& offset = 0)
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

//*******************************************************************************
// For l-values, non owning
template<typename IteratorType>
class IteratorCounterRange_lval
{
protected:
    using Iterator = IteratorCounter<IteratorType>;
    using IndexType = typename std::iterator_traits<IteratorType>::difference_type;
    
    IteratorType first;
    IteratorType last;
    IndexType    offset = 0;
    
    IteratorCounterRange_lval() {}
    
public:
    explicit IteratorCounterRange_lval(IteratorType begin, IteratorType end, const IndexType& offset = 0)
        : first(begin)
        , last(end)
        , offset(offset)
    {}
    
    Iterator begin() const { return Iterator(first, offset); }
    Iterator end() const { return Iterator(last, offset); }
};


//*******************************************************************************
// For r-values, needs to be owning

// Concepts
template<typename ContainerType>
concept ContainerIsNotInitializerList = not std::is_same_v<ContainerType, std::initializer_list<typename ContainerType::value_type>>;

template<typename ContainerType>
concept ContainerIsInitializerList = not ContainerIsNotInitializerList<ContainerType>;

// Template declaration
template<typename ContainerType, typename ContainerValueType = typename ContainerType::value_type>
class IteratorCounterRange_rval;

// For r-values, not std::initializer_list<T>
template<ContainerIsNotInitializerList ContainerType, typename ContainerValueType>
requires std::is_same_v<ContainerValueType, typename ContainerType::value_type>
class IteratorCounterRange_rval<ContainerType, ContainerValueType> : public IteratorCounterRange_lval<typename ContainerType::iterator>
{
private:
    using IteratorType = typename ContainerType::iterator;
    
    ContainerType owner;
    
public:
    IteratorCounterRange_rval() = delete;
    
    explicit IteratorCounterRange_rval(ContainerType&& container, const typename IteratorCounterRange_lval<IteratorType>::IndexType& offset = 0)
        : owner(container)
    {
        std::cout << "Container r-val range" << std::endl;
        
        IteratorCounterRange_lval<IteratorType>::offset = offset;
        
        IteratorCounterRange_lval<IteratorType>::first = std::begin(this->owner);
        IteratorCounterRange_lval<IteratorType>::last = std::end(this->owner);
    }
};

// For r-value std::initializer_list<T>. Special case since copying std::initializer_list is not advisable because of it's special properties
template<ContainerIsInitializerList ContainerType, typename ContainerValueType>
requires std::is_same_v<ContainerValueType, typename ContainerType::value_type>
class IteratorCounterRange_rval<ContainerType, ContainerValueType> : public IteratorCounterRange_lval<typename std::vector<ContainerValueType>::iterator>
{
private:
    using IteratorType = typename std::vector<ContainerValueType>::iterator;
    
    std::vector<ContainerValueType> owner; // std::initializer_list<ContainerValueType> will be copied into this std::vector

public:
    IteratorCounterRange_rval() = delete;

    explicit IteratorCounterRange_rval(ContainerType&& container, const typename IteratorCounterRange_lval<IteratorType>::IndexType& offset = 0)
        : owner(container)
    {
        std::cout << "List r-val range (special)" << std::endl;
            
        IteratorCounterRange_lval<IteratorType>::offset = offset;
        
        IteratorCounterRange_lval<IteratorType>::first = std::begin(this->owner);
        IteratorCounterRange_lval<IteratorType>::last = std::end(this->owner);
    }
};

//*******************************************************************************
// C-style array
template<typename T, std::size_t size>
decltype(auto) count(T (&arr)[size], const std::ptrdiff_t& offset = 0)
{
    return IteratorCounterRange_lval(arr, arr + size, offset);
}

//*******************************************************************************
// Range
template<typename IteratorType>
decltype(auto) count(const IteratorType& first, const IteratorType& last, const typename std::iterator_traits<IteratorType>::difference_type& offset = 0)
{
    return IteratorCounterRange_lval(first, last, offset);
}

//*******************************************************************************
// L-Value container & L-Value std::initializer_list
template<typename ContainerType>
decltype(auto) count(ContainerType& container, const typename std::iterator_traits<typename ContainerType::iterator>::difference_type& offset = 0)
{
    return IteratorCounterRange_lval(std::begin(container), std::end(container), offset);
}


//*******************************************************************************
//*******************************************************************************
// R-Value container
template<typename ContainerType>
decltype(auto) count(ContainerType&& container, const typename std::iterator_traits<typename ContainerType::iterator>::difference_type& offset = 0)
{
    return IteratorCounterRange_rval<ContainerType>(std::move(container), offset);
}

//*******************************************************************************
// R-Value std::initializer_list (special case).
// Needed, since copying a std::initializer_list is not adviseable.
// The list will be assigned to a std::vector, which will get counted
template<typename T>
decltype(auto) count(std::initializer_list<T>&& lst, const std::ptrdiff_t& offset = 0)
{
    return IteratorCounterRange_rval<std::initializer_list<T>>(std::move(lst), offset);
}

#endif
