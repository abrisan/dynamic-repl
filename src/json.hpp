//
//  json.hpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 30/09/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#ifndef json_hpp
#define json_hpp

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <tuple>
#include "json_container.hpp"

#define JSON_ADD_SUCC 0
#define JSON_ADD_FAIL_DUPLICATE 1

#ifndef JSON_TOKENS
#define JSON_TOKENS
#define JSON_CURLY_OPEN '{'
#define JSON_CURLY_CLOSE '}'
#define JSON_COLON ':'
#define JSON_COMMA ','
#define JSON_QUOTE '"'
#define JSON_CHAR_OR_COLON 'c'
#define JSON_WHITESPACE ' '
#endif

typedef int JSON_PARSE_RESULT;
typedef std::shared_ptr<int> i_ptr;
typedef std::shared_ptr<std::string> str_ptr;


typedef std::shared_ptr<json_container> json_ptr;

typedef std::tuple<str_ptr, i_ptr, json_ptr> json_value;

class json_object
{
private:
    std::unique_ptr<std::string> name;
    json_container information;
    std::unordered_map<std::string, std::shared_ptr<json_object>> child_objects;
    bool check_existing_name(std::string const &name) const;
    std::string to_string_helper(int tab_szie) const;
public:
    json_value operator[](std::string const &name);
    JSON_PARSE_RESULT add_string_attribute(std::string const &name, std::string const &attribute);
    JSON_PARSE_RESULT add_int_attribute(std::string const &name, int const &attribute);
    JSON_PARSE_RESULT add_json_object(std::string const &name, std::shared_ptr<json_object> const &object);
    std::string to_string() const;
    void set_name(std::string const &name);
    std::string const get_name() const {return *name;}
};



std::shared_ptr<json_object> parse_json_string(std::shared_ptr<std::vector<std::string>> json_string);

inline std::ostream& operator<<(std::ostream &cout, json_object const &my_obj)
{
    cout << my_obj.to_string();
    return cout;
}



#endif /* json_hpp */
