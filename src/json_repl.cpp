//
//  json_repl.cpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 04/10/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#include "json_repl.hpp"
#include "globals.hpp"
#include "json.hpp"

#include <iostream>
#include <vector>
#include <sstream>

struct json_syscall
{
    std::string message;
    int code;
};

json_syscall invalid_json_syscall()
{
    json_syscall ret;
    ret.message = "Invalid json entered";
    ret.code = JSON_INVALID_SYSCALL;
    return ret;
}

std::shared_ptr<std::vector<std::string>> read_json_strings()
{
    std::vector<std::string> *vec = new std::vector<std::string>;
    
    std::string s;
    std::stack<char> brackets;
    
    do
    {
        std::getline(std::cin, s);
        size_t strlen = s.length();
        for (size_t i = 0 ; i < strlen ; ++i)
        {
            if (s[i] == JSON_CURLY_OPEN)
                brackets.push(JSON_CURLY_OPEN);
            
            else if(s[i] == JSON_CURLY_CLOSE)
            {
                if (!brackets.size())
                    throw "Invalid JSON FORMAT";
                brackets.pop();
            }
        }
        vec -> push_back(s);
        s.clear();
    } while(brackets.size());
    
    return std::shared_ptr<std::vector<std::string>>(vec);
}

json_syscall json_eval(std::string s)
{
    json_syscall ret;
    
    auto split_string = split(s, " ");
    std::string com_keyword = (*split_string)[0];
    
    if (com_keyword == NEW_OBJECT)
    {
        if (split_string -> size() < 2)
            return invalid_json_syscall();
        
        std::shared_ptr<std::vector<std::string>> strs = read_json_strings();
        
        auto obj = parse_json_string(strs);
        obj -> set_name((*split_string)[1]);
        loaded_json_objects.push_back(obj);
        
        ret.message = "Added object to memory. Run $ @delete (obj_name) to remove from memory";
        ret.code = JSON_NORM_SYSCALL;
    }
    else if (com_keyword == RELEASE_OBJECT)
    {
        size_t veclen = loaded_json_objects . size();
        size_t index = -1;
        
        for (size_t i = 0 ; i < veclen ; ++i)
        {
            if (loaded_json_objects[i] -> get_name() == (*split_string)[1])
            {
                index = i;
                break;
            }
        }
        
        loaded_json_objects . erase(loaded_json_objects . begin() + index);
        
        ret.message = "Removed object from memory";
        ret.code = JSON_NORM_SYSCALL;
    }
    else if (com_keyword == SHOW_OBJECTS)
    {
        std::ostringstream retstr;
        
        for (auto const &x : loaded_json_objects)
        {
            retstr << x -> to_string() << "\n";
        }
        
        ret.message = retstr.str();
        ret.code = JSON_NORM_SYSCALL;
    }
    else if (com_keyword == QUIT_JSON)
    {
        ret.message = "Exiting JSON entry mode. Objects are still and memory and can be referenced in the REPL";
        ret.code = JSON_EXIT_SYSCALL;
    }
    
    return ret;
}

void run_json_repl()
{
    while (true)
    {
        std::cout << "JSONi $ ";
        std::string s;
        std::getline(std::cin, s);
        json_syscall res = json_eval(s);
        if (res.code == JSON_EXIT_SYSCALL)
            return;
        std:: cout << res.message << std::endl;
    }
}
