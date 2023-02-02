# Range-Based For Loop with counter variable

###### Small single header utility to add an integer counter/index variable to C++ Range-Based For Loops. Full cross platform support.

**Full functionality will be completed in the next days.**

## Contents
- [Simple example](#simple-example)
- [Description](#description)
    - [Motivation](#motivation)
    - [Supported container types](#supported-container-types)
- [Usage](#usage)
    - [Installation](#installation)
    - [Basic usage](#basic-usage)
        - [STL Container](#stl-container)
        - [Iterators](#iterators)
        - [C-Style Arrays](#c-style-arrays)
        - [r-Values](#r-values)
        - [Initializer list](#initializer-list)
    - [Using an offset for index](#using-an-offset-for-index)
    - [Reverse counting direction](#reverse-counting-direction)
- [Count function overview](#count-function-overview)

*See also: [License (zlib)](LICENSE.md)*

## Simple example
```cpp
std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

for(auto [value, index] : count(vec))
    std::cout << index << ": " << value << std::endl; // Index will be incremented automatically
```

Where `value` is of type `std::string`, and `index` is of type `std::iterator_traits<std::vector<std::string>::iterator>::difference_type`, which can be implicitly casted to an `int`.

##### Console output:
```
0: Element 1
1: Element 2
2: Element 3
```

## Description
With this small header utility you can easily add an index variable to Range Based For Loops, without any verbose code.
You can basically use any STL-Container, `std::initializer_list` or custom types derived from them. l-Values and r-Values are both supported. Additionally you can specify a counter offset, to start counting at a different value than zero.

*Reverse counting (start index at number of elements in container and count down to zero) is currently in development and will be added shortly.*

### Motivation
While declaring a separate counter variable just above the Range Based For Loop works, it adds quite some verbosity to the code. Also the counter variable's scope would be valid outside of the loop to, which can lead to some nasty name clashes.
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
| Associative | `std::set` <br> `std::map` <br> `std::multiset` <br> `std::multimap` <br> `std::unordered_set` <br> `std::unordered_map` <br> `std::unordered_multiset` <br> `std::unordered_multimap` | ⚠️ In development |
| Adaptors | `std::stack` <br> `std::queue` <br> `std::priority_queue` <br> `std::flat_set` <br> `std::flat_map` <br> `std::flat_multiset` <br> `std::flat_multimap`| ⚠️ In development |
| Special | `std::initializer_list` <br> `std::iterators` | ✅ Yes |

## Usage

### Installation
Include the `RangeForLoopWithCounter.h` header and you're ready to go. Requires C++20.

```cpp
#include "RangeForLoopWithCounter.h"
```

### Basic usage
Simply use the `count(...)` function for everything. See [the documentation of the count function](#count-function-overview) down below for further info.

- #### STL Container
    Usage with STL-Containers is straightforward:
    ```cpp
    std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

    for(auto [value, index] : count(vec))
        std::cout << index << ": " << value << std::endl;
    ```

- #### Associative containers like `std::map`
    This is currently in development and will be added in the next few days.

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
    The `count` function also fully supports r-Values and move semantics. However due to the design of Range Based For Loops in C++ (they destroy every temporary before actually running), `count` must become an *owning view* for r-Values. This means that what ever r-Value you pass to `count` will be copied inside of it.
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
`count(Container, Offset)` function:

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

### Reverse counting direction
Currently in development. Will be added in the next few days.

## Count function overview
The `count` function provides different overloads for usage with different types. The general usage is `count(Container, Offset, Reverse)`:
- `Container` is any type of container or array.
- `Offset` is the offset from which to start counting.

In development:
- ⚠️`Reverse` is a boolean to enable counting in reverse (start at highest element down to zero).

The `count` function is a `constexpr`.

##### The overloads are:

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
