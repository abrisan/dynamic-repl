//
//  json.cpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 30/09/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#include "json.hpp"
#include <sstream>
#include <iostream>

json_value json_object::operator[](std::string const &name)
{
    auto constit_str = information.string_values.find(name);
    if (constit_str != information.string_values.end())
        return std::make_tuple(str_ptr(new std::string(constit_str->second)), nullptr, nullptr);
    
    auto constit_int = information.int_values.find(name);
    if (constit_int != information.int_values.end())
        return std::make_tuple(nullptr, i_ptr(new int(constit_int->second)), nullptr);
    
    auto constit_obj = child_objects.find(name);
    if (constit_obj != child_objects.end())
        return std::make_tuple(nullptr, nullptr, nullptr);
    
    throw "Key Not Found";
}

JSON_PARSE_RESULT json_object::add_json_object(std::string const &name, std::shared_ptr<json_object> const &obj)
{
    if (check_existing_name(name))
        return JSON_ADD_FAIL_DUPLICATE;
    child_objects[name] = obj;
    return JSON_ADD_SUCC;
}

JSON_PARSE_RESULT json_object::add_int_attribute(std::string const &name, int const &value)
{
    if (check_existing_name(name))
        return JSON_ADD_FAIL_DUPLICATE;
    information.int_values[name]  = value;
    return JSON_ADD_SUCC;
}

JSON_PARSE_RESULT json_object::add_string_attribute(std::string const &name, std::string const &attribute)
{
    if (check_existing_name(name))
        return JSON_ADD_FAIL_DUPLICATE;
    information.string_values[name] = attribute;
    return JSON_ADD_SUCC;
}

bool json_object::check_existing_name(std::string const &name) const
{
    auto constit_str = information.string_values.find(name);
    if (constit_str != information.string_values.end())
        return true;
    
    auto constit_int = information.int_values.find(name);
    if (constit_int != information.int_values.end())
        return true;
   
    auto constit_obj = child_objects.find(name);
    if (constit_obj != child_objects.end())
        return true;
    
    return false;
}

std::string json_object::to_string() const
{
    std::ostringstream outstr;
    if (this -> name == nullptr)
    {
        outstr << "[object Object]";
    }
    else
    {
        outstr << "[" << this -> name << " Object]";
    }
    
    outstr << ": {";
    outstr << this -> to_string_helper(2);
    return outstr.str();
}

std::string json_object::to_string_helper(int tab_size) const
{
    std::ostringstream outstring;
    if (tab_size == 0)
        outstring << "{";
    outstring << "\n";
    for (auto const &x : child_objects)
    {
        for (int i = 0 ; i < tab_size ; ++i)
        {
            outstring << " " ;
        }
        outstring << x.first << ": {";
        outstring << x.second->to_string_helper(tab_size + 2);
        auto constit = child_objects.find(x.first);
        if (constit == child_objects.end())
        {
            outstring << "\n";
        }
        else
        {
            outstring << ",\n";
        }
    }
    for (auto const &x : information.int_values)
    {
        for (int i = 0 ; i < tab_size ; ++i)
        {
            outstring << " " ;
        }
        outstring << x.first << ": " << x.second << " (int),\n";
    }
    for (auto const &x : information.string_values)
    {
        for (int i = 0 ; i < tab_size ; ++i)
        {
            outstring << " " ;
        }
        outstring << x.first << ": " << x.second << " (std::string),\n";
    }
    for (int i = 0 ; i < tab_size - 2 ; ++i)
    {
        outstring << " " ;
    }
    outstring << "}";
    return outstring.str();
}

std::unique_ptr<int> try_to_convert_to_number(std::string const &value_string)
{
    size_t len = value_string.length();
    for (size_t i = 0 ; i < len ; ++i)
    {
        if (!std::isdigit(value_string[i]) && value_string[i] != '.')
            return nullptr;
    }
    int *ret = new int;
    (*ret) = atoi(value_string.c_str());
    return std::unique_ptr<int>(ret);
}

void process_key_and_value(std::stack<json_object *> ret, std::ostringstream &key, std::ostringstream &value)
{
    std::string value_string = value.str();
    auto is_number = try_to_convert_to_number(value_string);
    if (is_number == nullptr)
    {
        JSON_PARSE_RESULT res = ret.top() -> add_string_attribute(key.str(), value.str());
        if (res == JSON_ADD_FAIL_DUPLICATE)
            throw "Trying to add duplicate JSON Attribute";
    }
    else
    {
        JSON_PARSE_RESULT res = ret.top() -> add_int_attribute(key.str(), *is_number);
        if (res == JSON_ADD_FAIL_DUPLICATE)
        {
            std::cout << key.str() << " : " << value.str() << std::endl;
            throw "Trying to add duplicate JSON Attribute";
        }
        
    }
    key.str("");
    value.str("");
}

std::unique_ptr<std::unordered_map<std::string, std::string>> get_elements(std::string const &joined_string)
{
    std::unordered_map<std::string, std::string> *ret = new std::unordered_map<std::string, std::string>;
    
    std::ostringstream key;
    std::ostringstream value;
    std::ostringstream *cur_stream = &key;
    
    size_t string_len = joined_string.length();
    
    bool string_mode = false;
    
    std::stack<char> curly_braces;
    
    for(size_t i = 1 ; i < string_len - 1 ; ++i)
    {
        if (joined_string[i] == ' ')
            continue;
        
        else if (joined_string[i] == ',' && !string_mode && !curly_braces.size())
        {
            (*ret)[key.str()] = value.str();
            key.str("");
            value.str("");
            cur_stream = &key;
        }
        
        else if (joined_string[i] == '"' && !curly_braces.size())
        {
            string_mode = !string_mode;
        }
        
        else if (joined_string[i] == ':' && !curly_braces.size())
        {
            cur_stream = &value;
        }
        
        else if (joined_string[i] == '{')
        {
            (*cur_stream) << joined_string[i];
            curly_braces.push('{');
        }
        
        else if (joined_string[i] == '}')
        {
            (*cur_stream) << joined_string[i];
            curly_braces.pop();
        }
        
        else
        {
            (*cur_stream) << joined_string[i];
        }
    }
    
    if (key.str().length() > 0 && value.str().length() > 0)
    {
        (*ret)[key.str()] = value.str();
    }
    
    return std::unique_ptr<std::unordered_map<std::string, std::string>>(ret);
}

std::string join_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
    std::ostringstream joined_json_string;
    
    for (auto const &x : *json_string)
    {
        joined_json_string << x;
    }
    
    return joined_json_string.str();
}

std::shared_ptr<json_object> new_json_parse(std::shared_ptr<std::vector<std::string>> json_string)
{
    std::string joined_string = join_json_string(json_string);
    
    size_t length = joined_string.size();
    
    std::ostringstream key;
    std::ostringstream value;
    std::ostringstream *cur_stream = &key;
    
    std::stack<json_object *> objects;
    
    for (size_t i = 0 ; i < length ; ++i)
    {
         
    }
    
    return nullptr;
}


std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
    if (json_string == nullptr)
        return std::shared_ptr<json_object>(new json_object);
    
    std::ostringstream joined_json_string;
    
    for (auto const &x : *json_string)
    {
        joined_json_string << x;
    }
    
    std::string joined_string = joined_json_string.str();
    
    std::unique_ptr<std::unordered_map<std::string, std::string>> elements = get_elements(joined_string);
    
    json_object *ret = new json_object;
    
    for (auto const &x : *elements)
    {
        if (x.second[0] != '{')
        {
            auto try_number = try_to_convert_to_number(x.second);
            if (try_number == nullptr)
            {
                ret -> add_string_attribute(x.first, x.second);
            }
            else
            {
                ret -> add_int_attribute(x.first, *try_number);
            }
        }
        else
        {
            std::vector<std::string> *to_pass = new std::vector<std::string>;
            to_pass -> push_back(x.second);
            std::shared_ptr<std::vector<std::string>> arg(to_pass);
            ret -> add_json_object(x.first, parse_json_string(arg));
        }
    }
    
    return std::shared_ptr<json_object>(ret);
}
