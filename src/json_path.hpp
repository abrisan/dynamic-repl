#ifndef JSON_PATH_H
#define JSON_PATH_H

#include <string>
#include <vector>
#include "json.hpp"
#include "globals.hpp"

#ifndef JSON_PATH_CHARS
#define JSON_PATH_CHARS
#define JSON_PATH_PERIOD '.'
#define JSON_PATH_OP_BEGIN '['
#define JSON_PATH_OP_END ']'
#endif

class json_path
{
private:
    std::vector<std::string> path;
    int current_index = 0;
public:
    void parse_json_path(std::string const &path);
    json_value evaluate_path(global_context &context);
    json_value evaluate_path(json_object &object);
    std::string to_string();
    void clear() {path.clear();}
};


#endif
