//
//  globals.cpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include "globals.hpp"
#include <iostream>

std::unique_ptr<std::vector<std::string>> split(std::string const &input, char const *delimiter)
{
    char delim;
    if (delimiter == nullptr)
    {
        delim = ' ';
    }
    else
    {
        delim = *delimiter;
    }
    
    size_t i = 0;
    
    std::unique_ptr<std::vector<std::string>> result(new std::vector<std::string>);
    
    std::string s;
    
    while (i < input.size())
    {
        if (input[i] != delim)
        {
            s += input[i];
        }
        else
        {
            result->push_back(s);
            s.clear();
        }
        ++i;
    }
        
    if (s != "")
    {
        result->push_back(s);
    }
        
    return result;
}

