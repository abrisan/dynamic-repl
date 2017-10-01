//
//  json.cpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 30/09/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#include "json.hpp"

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
    return JSON_ADD_SUCC;
}
