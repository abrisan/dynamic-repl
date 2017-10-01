//
//  options.cpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include "options.hpp"
#include <fstream>
#include <iostream>

bool is_valid_key(std::string const &key)
{
    for (auto const &opt : CONFIG_OPTIONS)
    {
        if (opt == key)
            return true;
    }
    return false;
}

PARSE_RESULT repl_options::process_option(std::string const &option_line)
{
    std::unique_ptr<std::vector<std::string>> result = split(option_line, " ");
    if (!result || result->size() > 2)
    {
        return FAIL;
    }
    
    std::string key = (*result)[0];
    std::string value = (*result)[1];
    
    if (!is_valid_key(key))
    {
        return FAIL;
    }
    else
    {
        (*this->selected_options)[key] = value;
    }
    
    return SUCC;
}

PARSE_RESULT repl_options::parse_file(std::string relative_path)
{
    std::ifstream cin(relative_path);
    
    std::string s;
    
    std::vector<std::string> ins;
        
    while(std::getline(cin, s))
    {
        PARSE_RESULT res = process_option(s);
                if (res == FAIL)
                        throw "Failed to process command";
    }
    
    return SUCC;
}

std::string repl_options::get_option_with_key(std::string const &key) const
{
    auto constit = this->selected_options->find(key);
    if (constit == this->selected_options->end())
        throw "Key not found";
    else
        return constit->second;
}
