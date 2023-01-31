# Range-Based For Loop with counter variable

###### Small single header utility adding an integer counter/index variable to C++ Range-Based For Loops.

**Full functionality will be completed in the next days.**

## Contents
- [Simple example](#simple-example)
- [Description](#desc)
    - [Motivation](#motivation)
    - [Supported container types](#supported-types)
- [Usage](#usage)
    - [Installation](#install)
    - [Basic usage](#basic)
        - [STL Container](#container)
        - [Iterators](#iterators)
        - [C-Style Arrays](#arrays)
        - [r-Values](#r-val)
        - [Initializer list](#init_lst)
    - [Using an offset for index](#offset)
    - [Reverse counting direction](#rev-count)

*See also: [License (zlib)](LICENSE.md)*

## Simple example
```cpp
std::vector<std::string> vec = {"Element 1", "Element 2", "Element 3"};

for(auto [value, index] : count(vec))
    std::cout << index << ": " << value << std::endl; // Index will be incremented automatically
```

##### Console output:
```
0: Element 1
1: Element 2
2: Element 3
```

## Description
With this small header utility you can easily add an index variable to Range Based For Loops, without any verbose code.
You can basically use any STL-Container, `std::initialzer_list` or custom types derived from them. l-Values and r-Values are both supported. Additionally you can specifiy a counter offset, to start counting at a different value than zero.

Reverse counting (start index at number of elements in container and count down to zero) is currently in development and will be added shortly.

### Motivation
While declaring a seperate counter variable just above the Range Based For Loop works, it adds quite some verbosity to the code. Also the counter variable's scope would be valid outside of the loop to, which can lead to some nasty name clashes.
With C++20 we got initialization in Range Based For Loops, but this also adds verbosity to the code by declaring a seperate variable and incrementing it:
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
Include the `RangeForLoopWithCounter.h` header. Requires C++20.

```cpp
#include "RangeForLoopWithCounter.h"
```
    
### Basic usage

- #### STL Container
    aaa

- #### Iterators
    bbb

- #### C-Style Arrays
    ccc

- #### r-Values
    ddd

- #### Initializer list
    An `std::initializer_list` can also be used (l-Values and r-Values):
    ```cpp
    for(auto [value, index] : count({"L1", "L2", "L3"}))
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


## Additional notes
Empty
