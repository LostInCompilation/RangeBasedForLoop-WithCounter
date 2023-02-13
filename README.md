# Range-Based For Loop with counter variable

![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS%20%7C%20anything%20else-blue?style=flat-square&&logo=Platform.sh) ![Version](https://img.shields.io/badge/Version-V1.1-brightgreen?style=flat-square&) ![CPP](https://img.shields.io/badge/Language-C++20-orange?style=flat-square&&logo=C%2b%2b)

###### ⚠️ README will be updated for the new version tomorrow.

##### Small single header utility to add an integer counter/index variable to C++ Range-Based For Loops. Full cross platform support.

## Contents
- [Simple example](#simple-example)
- [Description](#description)
    - [Motivation](#motivation)
    - [Supported container types](#supported-container-types)
- [Usage](#usage)
    - [Installation](#installation)
    - [Basic usage](#basic-usage)
        - [STL Container](#stl-container)
        - [Associative containers like `std::map`](#associative-containers-like-stdmap)
        - [Iterators](#iterators)
        - [C-Style Arrays](#c-style-arrays)
        - [r-Values](#r-values)
        - [Initializer list](#initializer-list)
    - [Using an offset for index](#using-an-offset-for-index)
    - [Reverse](#reverse)
        - [Reverse index](#reverse-index)
        - [Reverse elements](#reverse-elements)
- [Count-function overview](#count-function-overview)
    - [Parameters](#parameters)
    - [Return type and variable types](#return-type-and-variable-types)
    - [Overloads](#overloads)

*See also: [License (zlib)](LICENSE.md)*

## Simple example
```cpp
std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

for(auto [value, index] : count(vec))
    std::cout << index << ": " << value << std::endl; // Index will be incremented automatically
```

Where `value` is of type `std::string&`, and `index` is of type `const std::iterator_traits<std::vector<std::string>::iterator>::difference_type&`, which can be implicitly cast to an `int`.

##### Console output:
```
0: Element 1
1: Element 2
2: Element 3
```

## Description
With this small header utility you can easily add an index variable to Range Based For Loops, without any verbose code.
You can use any STL-Container, `std::initializer_list` or custom types derived from them. l-Values and r-Values are both supported.
Additionally you can specify a **counter offset**, to start counting at a different value than zero (see [Using an offset](#using-an-offset-for-index)).
**Reverse counting** is also supported in two different variants. Reversing the enumeration of the elements itself (start with last element in container) or just reverse the index variable (start index at number of elements in container and count down to zero). Both modes can be combined (see [Reverse](#reverse)).

### Motivation
While declaring a separate counter variable right above the Range Based For Loop would work, it adds quite some verbosity to the code. Also the counter variable's scope would be valid outside of the loop too, which can lead to some nasty name clashes.
With C++20 we got initialization in Range Based For Loops, but this also adds verbosity to the code by declaring a separate variable and incrementing it:
```cpp
for (int index = 0; auto& value : vec)
    i++; // Increment still needed
```

The above example can also introduce some nasty lifetime issues with more complex types, since Range Based For Loops destroy any temporaries before actually running.

I wanted to make a simple and universal solution, with almost no verbosity and the flexibility to count up or down and start counting based on an offset.

### Supported container types
Type | Container | Supported |
| -- | --------- | --------- |
| C-Style Array | `int myArr[42];` | ✅ Yes |
| Sequence | `std::array` <br> `std::vector` <br> `std::deque` <br> `std::forward_list` <br> `std::list` | ✅ Yes |
| Associative | `std::set` <br> `std::map` <br> `std::multiset` <br> `std::multimap` <br> `std::unordered_set` <br> `std::unordered_map` <br> `std::unordered_multiset` <br> `std::unordered_multimap` | ✅ Yes |
| Adaptors | `std::stack` <br> `std::queue` <br> `std::priority_queue` <br> `std::flat_set` <br> `std::flat_map` <br> `std::flat_multiset` <br> `std::flat_multimap`| ❌ No<br>These types aren't iterable and don't support Range Based For Loops[^1] |
| Special | `std::initializer_list` <br> `std::iterator` | ✅ Yes |

[^1]: You can use a workaround and copy, for example, a `std::queue` into a temporary `std::vector` which then can be used with the `count(...)` function. But this introduces run time overhead.

## Usage

### Installation
Include the `RangeForLoopWithCounter.h` header and use the `RBFLCounter` namespace and you're good to go. Requires **C++20**.

```cpp
#include "RangeForLoopWithCounter.h"

using namespace RBFLCounter;
```

### Basic usage
Simply use the `count(...)` or `rcount(...)` (reverse count) function for everything. See [the documentation of the count/rcount function](#count-function-overview) down below for further info.

- #### STL Container
    Usage with STL-Containers is straightforward:
    ```cpp
    std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

    for(const auto& [value, index] : count(vec))
        std::cout << index << ": " << value << std::endl;
    ```

- #### Associative containers like `std::map`
    ⚠️ Supported. README will be updated soon.

- #### Iterators
    You can also use `std::iterator` to specify a range. This will print  only the first two elements of `vec`.
    ```cpp
    for(auto [value, index] : count(vec.begin(), vec.begin() + 2))
        std::cout << index << ": " << value << std::endl;
    ```

- #### C-Style Arrays
    C-Style arrays can be used in the same way:
    ```cpp
    int arr[] = {42, 43, 44, 45, 46, 47};
    
    for(auto [value, index] : count(arr))
        std::cout << index << ": " << value << std::endl;
    ```

- #### r-Values
    The `count` and `rcount` functions fully support r-Values and move semantics. However due to the design of Range Based For Loops in C++ (they destroy every temporary before actually running), `count` and `rcount` must become an *owning view* for r-Values. This means that what ever r-Value you pass to `count` or `rcount` will be copied inside of it.
    ```cpp
    for(auto [value, index] : count(std::vector<std::string>{"X", "Y", "Z"}))
        std::cout << index << ": " << value << std::endl;
    ```

- #### Initializer list
    An `std::initializer_list` can also be used (l-Values and r-Values):
    ```cpp
    for(auto [value, index] : count({"L1", "L2", "L3"}))
        std::cout << index << ": " << value << std::endl;
    ```
    
### Using an offset for index
To use an offset for the counter variable, simply pass an offset to the 
`count(Container, Offset=0)` or `rcount(Container, Offset=0)` function:

```cpp
std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

for(auto [value, index] : count(vec, 42)) // Start counting at 42
    std::cout << index << ": " << value << std::endl;
```

The default offset value is zero.

##### Console output:
```
42: Element 1
43: Element 2
44: Element 3
```

### Reverse
There are two different types of reversing, which can also be combined:

- #### Reverse index
    index
- #### Reverse elements
    elements

To combine both modes, use `rcount` with the last parameter (reverse index) set to true.

## Count function overview
The `count` and `rcount` functions provide different overloads and parameters for usage with different types and to adjust the behaviour of the counting.
The `count` and `rcount` functions are `constexpr`.

### Parameters
The general usage is `count(Container, Offset=0, ReverseIndex=false)` and `rcount(Container, Offset=0, ReverseIndex=false)`:
- `Container` is any type of container or array.
- `Offset` is the offset from where to start counting. Default is zero.
- `ReverseIndex` is a boolean to enable counting in reverse for the index (start at number of elements in container, counting down to zero). Default is false (no reverse index counting).

### Return type and variable types
return

### Overloads

- C-Style arrays
    ```cpp
    template<typename T, std::size_t size>
    count(T (&arr)[size], const std::ptrdiff_t& offset = 0)
    ```

- Iterators
    ```cpp
    template<typename IteratorType>
    count(const IteratorType& first, const IteratorType& last, const typename std::iterator_traits<IteratorType>::difference_type& offset = 0)
    ```

    Note: There is no `rcount` function for iterators, since you can just pass a reverse iterator to the normal `count` function, which then behaves like `rcount`: `std::rbegin(...)` and `std::rend(...)`.

- l-Value container and l-Value `std::initializer_list`
    ```cpp
    template<typename ContainerType>
    count(ContainerType& container, const typename std::iterator_traits<typename ContainerType::iterator>::difference_type& offset = 0)
    ```

- r-Value container
    ```cpp
    template<typename ContainerType>
    count(ContainerType&& container, const typename std::iterator_traits<typename ContainerType::iterator>::difference_type& offset = 0)
    ```

- r-Value `std::initializer_list`
    ```cpp
    template<typename T>
    count(std::initializer_list<T>&& init_list, const std::ptrdiff_t& offset = 0)
    ```
