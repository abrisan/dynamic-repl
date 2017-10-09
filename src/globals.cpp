//
//  globals.cpp
//  TripSLang
//
//  Created by Alexandru Brisan on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include "globals.hpp"
#include <iostream>

std::unique_ptr<std::vector<std::string>> split(std::string const &input, std::string const &delimiter)
{
    size_t const delim_length = delimiter . length();

    size_t i = 0;

    std::unique_ptr<std::vector<std::string>> result(new std::vector<std::string>);

    std::string s;

    while (i < input.size())
    {
        bool found_delim = true;
        if (i + delim_length < input.size())
        {
          size_t k = 0;
          for (size_t j = i ; j < i + delim_length; ++j)
          {
            if (input[j] != delimiter[k])
            {
              found_delim = false;
              break;
            }
            ++k;
          }
        }
        else
        {
            found_delim = false;
        }
        if (!found_delim)
        {
            s += input[i];
            ++i;
        }
        else
        {
            result->push_back(s);
            s.clear();
            i += delim_length;
        }
    }

    if (s != "")
    {
        result->push_back(s);
    }

    return result;
}
