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
    std::ostringstream outstring;
    outstring << "{\n";
    for (auto const &x : child_objects)
    {
        outstring << x.first << ":\n{\n";
        for (auto const &y : x.second->information.int_values)
        {
            outstring << y.first << " : " << y.second << ",\n";
        }
        for (auto const &y : x.second->information.string_values)
        {
            outstring << y.first << " : " << y.second << ",\n";
        }
        outstring << "},\n";
    }
    for (auto const &x : information.int_values)
    {
        outstring << x.first << ": " << x.second << " (int),\n";
    }
    for (auto const &x : information.string_values)
    {
        outstring << x.first << ": " << x.second << " (std::string),\n";
    }
    outstring << "}\n";
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

std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
  if (json_string == nullptr)
    return std::shared_ptr<json_object>(new json_object);

  
  json_object *ret = new json_object;

    std::stack<json_object *> objects;
    objects.push(ret);
    
  std::stack<char> expected_chars;

    std::ostringstream joined_json_string;
    
    for (auto const &x : *json_string)
    {
        joined_json_string << x;
    }
    
    auto const string_at = joined_json_string.str();

    size_t const string_size = string_at.size();

    std::ostringstream key;
    std::ostringstream value;
    std::ostringstream *to_add = &key;

    
    bool string_mode = false;
    bool root_object_started = false;
    bool object_mode = false;
    
    for (size_t j = 0 ; j < string_size ; ++j)
    {
       if (string_at[j] == JSON_CURLY_OPEN)
      {
          if (root_object_started)
          {
              objects.push(new json_object);
          }
          else
              root_object_started = true;
        expected_chars.push(JSON_CURLY_OPEN);
        expected_chars.push(JSON_CHAR_OR_COLON);
      }
      else if (string_at[j] == JSON_CURLY_CLOSE)
      {
      
        if (expected_chars.top() == JSON_CURLY_OPEN)
        {
            process_key_and_value(objects, key, value);
            if (objects.size() > 1)
            {
                json_object *ptr = objects.top();
                objects.pop();
                JSON_PARSE_RESULT res = objects.top() -> add_json_object(key.str(), std::shared_ptr<json_object>(ptr));
                if (res == JSON_ADD_FAIL_DUPLICATE)
                    throw "Trying to add duplicate name";
            }
          to_add = &key;
          expected_chars.pop();
        }
        else
        {
            std::cout << expected_chars.top() << std::endl;
            throw "Found unexpected char";
        }
          
      }
      else if (string_at[j] == JSON_COLON)
      {
        if (expected_chars.top() == JSON_CHAR_OR_COLON)
        {
            to_add = &value;
            expected_chars.pop();
        }
          
        else
          throw "Found unexpected colon";
      }
        else if (string_at[j] == JSON_COMMA && (!string_mode))
        {
            if (expected_chars.top() == JSON_COMMA)
                expected_chars.pop();
            expected_chars.push(JSON_CHAR_OR_COLON);
            process_key_and_value(objects, key, value);
            to_add = &key;
        }
        else if (string_at[j] == JSON_WHITESPACE)
        {
            continue;
        }
        else if (string_at[j] == JSON_QUOTE)
        {
            string_mode = !string_mode;
            if (!string_mode)
                expected_chars.push(JSON_COMMA);
        }
      else
      {
          if (expected_chars.top() == JSON_COMMA)
              throw "Found unexpected character, expected comma";
        (*to_add) << (string_at[j]);
      }
    }

    return std::shared_ptr<json_object>(ret);
}
