//
//  json.hpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 30/09/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#ifndef json_hpp
#define json_hpp

#include "globals.hpp"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <tuple>

#define JSON_ADD_SUCC 0
#define JSON_ADD_FAIL_DUPLICATE 1

typedef int JSON_PARSE_RESULT;
typedef std::shared_ptr<int> i_ptr;
typedef std::shared_ptr<std::string> str_ptr;

struct json_container
{
    std::unordered_map<std::string, json_container> child_objects;
    std::unordered_map<std::string, std::string> string_values;
    std::unordered_map<std::string, int> int_values;
};

typedef std::shared_ptr<json_container> json_ptr;

typedef std::tuple<str_ptr, i_ptr, json_ptr> json_value;

class json_object
{
private:
    std::string name;
    json_container information;
public:
    json_value operator[](std::string const &name);
    JSON_PARSE_RESULT add_string_attribute(std::string const &name, std::string const &attribute);
    JSON_PARSE_RESULT add_int_attribute(std::string const &name, int const &attribute);
    JSON_PARSE_RESULT add_json_object(std::string const &name, json_object const &object);
};



std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string)
{
    
    return nullptr;
}

#endif /* json_hpp */
