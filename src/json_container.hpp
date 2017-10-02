#ifndef JSON_CONTAINER_H
#define JSON_CONTAINER_H

class json_container
{
public:
    std::unordered_map<std::string, std::string> string_values;
    std::unordered_map<std::string, int> int_values;
};
#endif
