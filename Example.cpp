/*
 
 EXAMPLE
 
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

// Compile example with way too many warnings enabled
// g++ -std=c++20 -fsanitize=address -static-libsan -g -Weverything -Wno-c++98-compat -Wno-pre-c++17-compat -Wno-padded -Wno-ctad-maybe-unsupported -Wno-missing-prototypes -o OutputBinary Example.cpp

#include "RangeForLoopWithCounter.h"

#include <iostream>
#include <vector>
#include <list>
#include <map>

using namespace RBFLCounter;

// Move semantics example
void RValueMoveExample(std::vector<int>&& v)
{
    std::cout << "std::vector move example" << std::endl << std::endl;
    
    for(auto [value, index] : count(std::move(v)))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
}

// Example of all possible reverse modes
void ReverseExample()
{
    std::cout << std::endl << "***********************************************" << std::endl;
    std::cout << "Reverse examples" << std::endl << std::endl;

    std::vector<std::string> vec = {"A", "B", "C", "D", "E"};
    
    //*************************************************************
    // Normal order
    std::cout << "Vector: Normal order (count())" << std::endl << std::endl;
    
    for(auto [value, index] : count(vec))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "***********************************************" << std::endl;

    //*************************************************************
    // Reverse value enumeration (rcount())
    std::cout << "Vector: Reverse value enumeration (rcount())" << std::endl << std::endl;
    
    for(auto [value, index] : rcount(vec))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "***********************************************" << std::endl;
    
    //*************************************************************
    // Reverse index counting (count())
    std::cout << "Vector: Reverse index counting (count())" << std::endl << std::endl;
    
    for(auto [value, index] : count(vec, 0, true))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "***********************************************" << std::endl;
    
    //*************************************************************
    // Reverse index counting and reverse value enumeration (rcount())
    std::cout << "Vector: Reverse index counting and reverse value enumeration (rcount())" << std::endl << std::endl;
    
    for(auto [value, index] : rcount(vec, 0, true))
        std::cout << index << ": " << value << std::endl;
}

int main()
{
    std::cout << "Range-Based for loop with counter - Example" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // C-style array example
    std::cout << "C-style array" << std::endl << std::endl;
    
    int arr[] = {42, 43, 44, 45, 46, 47};
    
    for(auto [value, index] : count(arr))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Vector example (l-value) with optional const specifier
    std::cout << "std::vector (l-Value)" << std::endl << std::endl;
    
    std::vector<std::string> vec = {"A", "B", "C", "D", "E", "F", "G"};
    
    for(const auto [value, index] : count(vec))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Iterator example of vector
    std::cout << "Iterator (std::vector)" << std::endl << std::endl;
    
    for(auto [value, index] : count(vec.begin(), vec.begin() + 3))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Vector example (r-value)
    std::cout << "std::vector (r-Value)" << std::endl << std::endl;
    
    for(auto [value, index] : count(std::vector<std::string>{"X", "Y", "Z"}))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // List example (l-value) with an offset of 100 (index starts at 100)
    std::cout << "std::list (l-Value) with an offset of 100" << std::endl << std::endl;
    
    std::list<std::string> list = {"L1", "L2", "L3", "L4", "L5"};
    
    for(auto [value, index] : count(list, 100))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Initializer List example (l-value)
    std::cout << "std::initializer_list (l-Value)" << std::endl << std::endl;
    
    std::initializer_list<int> init_list = {5, 6, 7, 8, 9};
    
    for(auto [value, index] : count(init_list))
        std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Initializer List example (r-value)
    std::cout << "std::initializer_list (r-Value)" << std::endl << std::endl;
    
    for(auto [value, index] : count({"R1", "R2", "R3", "R4", "R5"}))
            std::cout << index << ": " << value << std::endl;
    
    std::cout << std::endl << "-----------------------------------------------" << std::endl;
    
    //*************************************************************
    // Move semantics example
    RValueMoveExample(std::vector<int>{1, 2});
    
    //*************************************************************
    // Associative container: std::map example
    std::cout << "std::map" << std::endl << std::endl;
    
    std::map<float, std::string> testMap = {{3.14159f, "Pi"}, {2.71828f, "Euler's"}, {1.41421f, "Square-Root of 2"}};
    
    for(auto [element, index] : count(testMap))
        std::cout << index << ": Map(" << element.first << ", " << element.second << ")" << std::endl;
    
    //*************************************************************
    // Different reverse mode examples
    ReverseExample();
    
    return 0;
}
