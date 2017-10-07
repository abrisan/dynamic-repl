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
                    return nullptr;
                brackets.pop();
            }
        }
        vec -> push_back(s);
        s.clear();
    } while(brackets.size());

    return std::shared_ptr<std::vector<std::string>>(vec);
}

std::unique_ptr<json_syscall> check_existing_name(global_context &context, std::string const &name)
{
    for (auto const &x : context.get_json_names())
    {
        if (x == name)
        {
            json_syscall *ret = new json_syscall;
            ret -> message = "Object already exists";
            ret -> code = JSON_NORM_SYSCALL;
            return std::unique_ptr<json_syscall>(ret);
        }
    }

    return nullptr;
}


json_syscall json_eval(global_context &context, std::string s)
{
    json_syscall ret;

    auto split_string = split(s, " ");
    std::string com_keyword = (*split_string)[0];

    if (com_keyword == NEW_OBJECT)
    {
        if (split_string -> size() < 2)
            return invalid_json_syscall();

        auto existing_object = check_existing_name(context, split_string -> operator[](1));

        if (existing_object != nullptr)
        {
            return *existing_object;
        }

        std::shared_ptr<std::vector<std::string>> strs = read_json_strings();

        if (strs == nullptr)
            return invalid_json_syscall();

        auto obj = parse_json_string(strs);
        obj -> set_name((*split_string)[1]);
        context.loaded_json_objects().push_back(obj);

        context.get_json_names().push_back((*split_string)[1]);

        ret.message = "Added object to memory. Run $ @delete (obj_name) to remove from memory";
        ret.code = JSON_NORM_SYSCALL;
    }
    else if (com_keyword == RELEASE_OBJECT)
    {
        size_t veclen = context.loaded_json_objects() . size();
        size_t index = -1;

        for (size_t i = 0 ; i < veclen ; ++i)
        {
            if (context.loaded_json_objects()[i] -> get_name() == (*split_string)[1])
            {
                index = i;
                break;
            }
        }

        context.loaded_json_objects() . erase(context.loaded_json_objects() . begin() + index);

        ret.message = "Removed object from memory";
        ret.code = JSON_NORM_SYSCALL;
    }
    else if (com_keyword == SHOW_OBJECTS)
    {
        std::ostringstream retstr;

        for (auto const &x : context.loaded_json_objects())
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

void run_json_repl(global_context &context)
{
    while (true)
    {
        std::cout << "JSONi $ ";
        std::string s;
        std::getline(std::cin, s);
        json_syscall res = json_eval(context, s);
        std:: cout << res.message << std::endl;

        if (res.code == JSON_EXIT_SYSCALL)
            return;
    }
}
