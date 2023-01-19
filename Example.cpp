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

#include <iostream>
#include <vector>
#include <list>

#include "RangeForLoopWithCounter.h"

int main()
{
    std::cout << "Range-Based for loop with counter - Example" << std::endl;
    
    //*************************************************************
    // C-style array example
    int arr[] = {42, 43, 44, 45, 46, 47};
    
    for(auto [value, index] : count(arr))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // Vector example (l-value)
    std::vector<std::string> vec = {"A", "B", "C", "D", "E", "F", "G"};
    
    for(auto [value, index] : count(vec))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // Iterator example
    for(auto [value, index] : count(vec.begin(), vec.begin() + 3))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // List example (l-value) with an offset of 100 (index start at 100)
    std::list<std::string> list = {"L1", "L2", "L3", "L4", "L5"};
    
    for(auto [value, index] : count(list, 100))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // Vector example (r-value)
    //for(auto [value, index] : count(std::vector<std::string>{"X", "Y", "Z"}))
    //    std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // Initializer List example (l-value)
    std::initializer_list<int> init_lst = {5, 6, 7, 8, 9};
    
    for(auto [value, index] : count(init_lst))
        std::cout << value << ": " << index << std::endl;
    
    std::cout << std::endl << std::endl;
    
    //*************************************************************
    // Initializer List example (r-value)
    //for(auto [value, index] : count({10, 9, 8, 7, 6}))
    //    std::cout << value << ": " << index << std::endl;
    
    return 0;
}
