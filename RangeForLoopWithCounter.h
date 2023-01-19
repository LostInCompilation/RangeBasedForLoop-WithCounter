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
    
    IteratorType   iterator;
    IndexType      counter;
    
public:
    IteratorCounter() = delete;
    
    explicit IteratorCounter(IteratorType it, IndexType offset = 0)
        : iterator(it)
        , counter(offset)
    {}
    
    
};

#endif
