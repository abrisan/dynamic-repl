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
        return std::make_tuple(nullptr, nullptr, constit_obj -> second);

    return std::make_tuple(nullptr, nullptr, nullptr);
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
        outstr << "[" << *(this -> name) << " Object]";
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

void json_object::set_name(std::string const &name)
{
    this -> name = std::unique_ptr<std::string>(new std::string(name));
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

void process_key_and_value(json_object *&obj, std::string const &key, std::string const &value)
{
    if (!key.size() || !value.size())
        return;
    auto is_number = try_to_convert_to_number(value);
    if (is_number == nullptr)
    {
        JSON_PARSE_RESULT res = obj -> add_string_attribute(key, value);
        if (res == JSON_ADD_FAIL_DUPLICATE)
            throw "Trying to add duplicate JSON Attribute";
    }
    else
    {
        JSON_PARSE_RESULT res = obj -> add_int_attribute(key, *is_number);
        if (res == JSON_ADD_FAIL_DUPLICATE)
        {
            throw "Trying to add duplicate JSON Attribute";
        }

    }
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

void process_char(std::stack<char> *&expected, char const &c)
{

}

std::shared_ptr<json_object> parse_json_string_joined(std::string const &joined_string)
{
  size_t length = joined_string.size();

  std::ostringstream key;
  std::ostringstream value;
  std::ostringstream *cur_stream = &key;

  std::stack<json_object *> objects;
  std::stack<std::string> obj_names;

  bool t_string_mode = false;

  for (size_t i = 0 ; i < length ; ++i)
  {
      if (joined_string[i] == JSON_CURLY_OPEN)
      {
          objects.push(new json_object);
          obj_names.push(key.str());
          key.str("");
          value.str("");
          cur_stream = &key;
      }
      else if (joined_string[i] == JSON_CURLY_CLOSE)
      {
          if (!objects.size())
              return nullptr;
          json_object *top = objects.top();
          objects.pop();
          process_key_and_value(top, key.str(), value.str());
          cur_stream = &key;
          key.str("");
          value.str("");
          if (objects.size())
          {
              objects.top() -> add_json_object(obj_names.top(), std::shared_ptr<json_object>(top));
              obj_names.pop();
          }
          else
              return std::shared_ptr<json_object>(top);
      }
      else if (joined_string[i] == JSON_QUOTE)
      {
          t_string_mode = !t_string_mode;
      }
      else if (joined_string[i] == JSON_COMMA && !t_string_mode)
      {
          auto keystr = key.str();
          auto valstr = value.str();
          process_key_and_value(objects.top(), keystr, valstr);
          key.str("");
          value.str("");
          cur_stream = &key;
      }
      else if (joined_string[i] == JSON_COLON)
      {
          cur_stream = &value;
      }
      else if (joined_string[i] == JSON_WHITESPACE)
      {
          continue;
      }
      else
      {
          *cur_stream << joined_string[i];
      }

  }

  return nullptr;
}

std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
    std::string joined_string = join_json_string(json_string);

    return parse_json_string_joined(joined_string);
}
