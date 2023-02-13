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
#include <span>
#include <vector>
#include <initializer_list>

// Settings
//#define RBFL_FORCE_64BIT_COUNTER
#define RBFL_ENABLE_SIGNED_COUNTER // Only enable if you absolutely know what you're doing. For 64Bit not really an issue, but smaller types can result in overflows if handled incorrectly

#if defined(RBFL_ENABLE_SIGNED_COUNTER) && !defined(RBFL_FORCE_64BIT_COUNTER)
#pragma message ("Pragma warning")
#endif


// Range Based For Loop with Counter implementation
namespace RBFLCounter
{
#ifdef RBFL_FORCE_64BIT_COUNTER
    #ifdef RBFL_ENABLE_SIGNED_COUNTER
        using IndexType = int64_t;
    #else
        using IndexType = uint64_t;
    #endif
#else
    #ifdef RBFL_ENABLE_SIGNED_COUNTER
        using IndexType = std::ptrdiff_t;
    #else
        using IndexType = std::size_t;
    #endif
#endif

template<typename IteratorType>
class IteratorCounter
{
private:
    using IteratorReferenceType = typename std::iterator_traits<IteratorType>::reference;
    
    IteratorType m_Iterator;
    IndexType    m_Counter;
    const bool   m_ReverseIndex;
    
public:
    IteratorCounter() = delete;
    
    explicit IteratorCounter(const IteratorType& iterator, const IndexType& offset, const bool& reverseIndex)
        : m_Iterator(iterator)
        , m_Counter(offset)
        , m_ReverseIndex(reverseIndex)
    {}
    
    IteratorCounter& operator++()
    {
        m_Iterator++;
        
        if(m_ReverseIndex)
            m_Counter--;
        else
            m_Counter++;
        
        return *this;
    }
    
    std::pair<IteratorReferenceType, const IndexType&> operator*() const { return {*m_Iterator, m_Counter}; }
    
    bool operator==(const IteratorCounter& rhs) const { return m_Iterator == rhs.m_Iterator; }
    bool operator!=(const IteratorCounter& rhs) const { return m_Iterator != rhs.m_Iterator; }
};

//*******************************************************************************
//                  For l-values, non owning
template<typename IteratorType>
class IteratorCounterRange_lval
{
protected:
    using Iterator = IteratorCounter<IteratorType>;
    
    IteratorType m_First;
    IteratorType m_Last;
    
    const IndexType m_Offset;
    const bool      m_ReverseIndex;
    
    explicit IteratorCounterRange_lval(const IndexType& offset, const bool& reverseIndex) // Only to be used by derived classes
        : m_Offset(offset)
        , m_ReverseIndex(reverseIndex)
    {}
    
public:
    explicit IteratorCounterRange_lval(IteratorType begin, IteratorType end, const IndexType& offset, const bool& reverseIndex)
        : m_First(begin)
        , m_Last(end)
        , m_Offset(offset)
        , m_ReverseIndex(reverseIndex)
    {}
    
    Iterator begin() const { return Iterator(m_First, m_Offset, m_ReverseIndex); }
    Iterator end()   const { return Iterator(m_Last, m_Offset, m_ReverseIndex); }
};


//*******************************************************************************
//                      For r-values, needs to be owning
// Concepts
template<typename ContainerType>
concept ContainerIsInitializerList = std::is_same_v<ContainerType, std::initializer_list<typename ContainerType::value_type>>;

template<typename ContainerType>
concept ContainerIsNotInitializerList = not ContainerIsInitializerList<ContainerType>;

// Template declaration
template<typename ContainerType, typename IteratorType, typename ContainerValueType = typename ContainerType::value_type>
class IteratorCounterRange_rval;

// For r-Values, not std::initializer_list<T>
template<ContainerIsNotInitializerList ContainerType, typename IteratorType, typename ContainerValueType>
requires std::is_same_v<ContainerValueType, typename ContainerType::value_type> and (std::is_same_v<IteratorType, typename ContainerType::iterator> or std::is_same_v<IteratorType, typename ContainerType::reverse_iterator>)
class IteratorCounterRange_rval<ContainerType, IteratorType, ContainerValueType> : public IteratorCounterRange_lval<IteratorType>
{
private:
    ContainerType owner;
    
public:
    IteratorCounterRange_rval() = delete;
    
    explicit IteratorCounterRange_rval(ContainerType&& container, const IndexType& offset, const bool& reverseIndex)
        : IteratorCounterRange_lval<IteratorType>(offset, reverseIndex)
        , owner(container)
    {
        if constexpr (std::is_same_v<IteratorType, typename ContainerType::iterator>)
        {
            // Forward
            IteratorCounterRange_lval<IteratorType>::m_First = std::begin(owner);
            IteratorCounterRange_lval<IteratorType>::m_Last = std::end(owner);
        }
        else if constexpr (std::is_same_v<IteratorType, typename ContainerType::reverse_iterator>)
        {
            // Backward
            IteratorCounterRange_lval<IteratorType>::m_First = std::rbegin(owner);
            IteratorCounterRange_lval<IteratorType>::m_Last = std::rend(owner);
        }
    }
};

// For r-Value std::initializer_list<T>. Special case since copying std::initializer_list is not advisable because of it's special properties.
// Use a std::vector<T> and it's iterator as the owning type.
template<ContainerIsInitializerList ContainerType, typename IteratorType, typename ContainerValueType>
requires std::is_same_v<ContainerValueType, typename ContainerType::value_type> and (std::is_same_v<IteratorType, typename std::vector<ContainerValueType>::iterator> or std::is_same_v<IteratorType, typename std::vector<ContainerValueType>::reverse_iterator>)
class IteratorCounterRange_rval<ContainerType, IteratorType, ContainerValueType> : public IteratorCounterRange_lval<IteratorType/*typename std::vector<ContainerValueType>::iterator*/>
{
private:
    std::vector<ContainerValueType> owner; // std::initializer_list<ContainerValueType> will be copied into this std::vector<ContainerValueType>
    
public:
    IteratorCounterRange_rval() = delete;
    
    explicit IteratorCounterRange_rval(ContainerType&& container, const IndexType& offset, const bool& reverseIndex)
        : IteratorCounterRange_lval<IteratorType>(offset, reverseIndex)
        , owner(container)
    {
        if constexpr (std::is_same_v<IteratorType, typename std::vector<ContainerValueType>::iterator>)
        {
            // Forward
            IteratorCounterRange_lval<IteratorType>::m_First = std::begin(owner);
            IteratorCounterRange_lval<IteratorType>::m_Last = std::end(owner);
        }
        else if constexpr (std::is_same_v<IteratorType, typename std::vector<ContainerValueType>::reverse_iterator>)
        {
            // Backward
            IteratorCounterRange_lval<IteratorType>::m_First = std::rbegin(owner);
            IteratorCounterRange_lval<IteratorType>::m_Last = std::rend(owner);
        }
    }
};

//*******************************************************************************
// Iterator
template<typename IteratorType> // std::iterator_traits<IteratorType>::difference_type
constexpr decltype(auto) count(const IteratorType& first, const IteratorType& last, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_lval(first, last,
                                     reverseIndex ? (offset + static_cast<IndexType>(std::abs(std::distance(first, last))) - 1) : (offset),
                                     reverseIndex);
}

//*******************************************************************************
// C-style array
template<typename T, std::size_t size> // std::ptrdiff_t
constexpr decltype(auto) count(T (&arr)[size], const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_lval(std::begin(std::span<T>(arr)), std::end(std::span<T>(arr)),
                                     reverseIndex ? (offset + static_cast<IndexType>(size) - 1) : (offset),
                                     reverseIndex);
}

template<typename T, std::size_t size> // std::ptrdiff_t
constexpr decltype(auto) rcount(T (&arr)[size], const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_lval(std::rbegin(std::span<T>(arr)), std::rend(std::span<T>(arr)),
                                     reverseIndex ? (offset + static_cast<IndexType>(size) - 1) : (offset),
                                     reverseIndex);
}

//*******************************************************************************
// l-Value container and l-Value std::initializer_list
template<typename ContainerType> // std::iterator_traits<typename ContainerType::iterator>::difference_type
constexpr decltype(auto) count(ContainerType& container, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_lval(std::begin(container), std::end(container),
                                     reverseIndex ? (offset + static_cast<IndexType>(container.size()) - 1) : (offset),
                                     reverseIndex);
}

template<typename ContainerType> // std::iterator_traits<typename ContainerType::iterator>::difference_type
constexpr decltype(auto) rcount(ContainerType& container, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_lval(std::rbegin(container), std::rend(container),
                                     reverseIndex ? (offset + static_cast<IndexType>(container.size()) - 1) : (offset),
                                     reverseIndex);
}


//*******************************************************************************
//-------------------------------------------------------------------------------
//*******************************************************************************
// r-Value container
template<typename ContainerType> // std::iterator_traits<typename ContainerType::iterator>::difference_type
constexpr decltype(auto) count(ContainerType&& container, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_rval<ContainerType,
        typename ContainerType::iterator,
        typename ContainerType::value_type>(std::move(container),
                                            reverseIndex ? (offset + static_cast<IndexType>(container.size()) - 1) : (offset),
                                            reverseIndex);
}

template<typename ContainerType> // std::iterator_traits<typename ContainerType::iterator>::difference_type
constexpr decltype(auto) rcount(ContainerType&& container, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_rval<ContainerType,
        typename ContainerType::reverse_iterator,
        typename ContainerType::value_type>(std::move(container),
                                            reverseIndex ? (offset + static_cast<IndexType>(container.size()) - 1) : (offset),
                                            reverseIndex);
}

//*******************************************************************************
// r-Value std::initializer_list<T> (special case).
// This is needed, since copying a std::initializer_list is not adviseable.
// The list will be copied into a std::vector, which then will get iterated.
template<typename T> // std::ptrdiff_t
constexpr decltype(auto) count(std::initializer_list<T>&& init_list, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_rval<std::initializer_list<T>,
        typename std::vector<T>::iterator,
        T>(std::move(init_list),
           reverseIndex ? (offset + static_cast<IndexType>(init_list.size()) - 1) : (offset),
           reverseIndex);
}

template<typename T> // std::ptrdiff_t
constexpr decltype(auto) rcount(std::initializer_list<T>&& init_list, const IndexType& offset = 0, const bool& reverseIndex = false)
{
    return IteratorCounterRange_rval<std::initializer_list<T>,
        typename std::vector<T>::reverse_iterator,
        T>(std::move(init_list),
           reverseIndex ? (offset + static_cast<IndexType>(init_list.size()) - 1) : (offset),
           reverseIndex);
}

} // RBFLCounter

#endif // RANGE_FOR_LOOP_WITH_COUNTER_H
