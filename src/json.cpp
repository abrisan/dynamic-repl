//
//  json.cpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 30/09/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#include "json.hpp"
#include <sstream>

json_value json_object::operator[](std::string const &name)
{
    auto constit_str = information.string_values.find(name);
    if (constit_str != information.string_values.end())
        return std::make_tuple(str_ptr(&constit_str->second), nullptr, nullptr);
    return json_value();
}

JSON_PARSE_RESULT json_object::add_json_object(std::string const &name, json_object const &obj)
{
    return JSON_ADD_SUCC;
}

JSON_PARSE_RESULT json_object::add_int_attribute(std::string const &name, int const &value)
{
    return JSON_ADD_SUCC;
}

JSON_PARSE_RESULT json_object::add_string_attribute(std::string const &name, std::string const &attribute)
{
    auto constit_str = information.string_values.find(name);
    if (constit_str != information.string_values.end())
      return JSON_ADD_FAIL_DUPLICATE;
    information.string_values[name] = attribute;
    return JSON_ADD_SUCC;
}

std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
  if (json_string == nullptr)
    return std::shared_ptr<json_object>(new json_object);

  size_t i = 0;
  size_t length = json_string -> size();

  json_object *ret = new json_object;

  std::stack<char> expected_chars;

  while (i < length)
  {
    std::string const string_at = (*json_string)[i];
    size_t const string_size = string_at.size();

    std::ostringstream key;
    std::ostringstream value;
    std::ostringstream *to_add = &key;

    for (size_t j = 0 ; j < string_size ; ++i)
    {
      if (string_at[i] == JSON_CURLY_OPEN)
      {
        expected_chars.push(JSON_CURLY_OPEN);
        expected_chars.push(JSON_CHAR_OR_COLON);
      }
      else if (string_at[i] == JSON_CURLY_CLOSE)
      {
        if (expected_chars.top() == JSON_CURLY_OPEN)
        {
          JSON_PARSE_RESULT res = ret -> add_string_attribute(key.str(), value.str());
          if (res == JSON_ADD_FAIL_DUPLICATE)
            throw "Trying to add duplicate element to JSON";
          key.str("");
          value.str("");
          to_add = &key;
          expected_chars.pop();
        }
        else
          throw "Found unexpected char";
      }
      else if (string_at[i] == JSON_COLON)
      {
        if (expected_chars.top() == JSON_CHAR_OR_COLON)
          to_add = &value;
        else
          throw "Found unexpected colon";
      }
      else
      {
        (*to_add) << (string_at[i]);
      }
    }

    ++i;
  }
}
