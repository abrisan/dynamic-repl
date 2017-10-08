#include "json_path.hpp"

#include <sstream>

void json_path::parse_json_path(std::string const &input)
{
    size_t length = input.size();
    std::ostringstream strm;
    bool ignore_period = false;
    for (size_t i = 0 ; i < length ; ++i)
    {
        if (input[i] == JSON_PATH_PERIOD)
        {
            if (ignore_period)
            {
                ignore_period = false;
                continue;
            }
            this -> path.push_back(strm.str());
            strm.str("");
        }
        else if (input[i] == JSON_PATH_OP_BEGIN)
        {
            this -> path.push_back(strm.str());
            strm.str("");
        }
        else if (input[i] == JSON_PATH_OP_END)
        {
            this -> path.push_back(strm.str());
            strm.str("");
            ignore_period = true;
        }
        else
        {
          strm << input[i];
        }
    }
    if (strm.str().size())
    {
        this -> path . push_back(strm.str());
    }
}

std::string json_path::to_string()
{
    std::ostringstream out;
    for (auto const &x : path)
    {
        out << x << "\n";
    }
    return out.str();
}

json_value json_path::evaluate_path(global_context &context)
{
    for (auto const &x : context.loaded_json_objects())
    {
        if (x -> get_name() == path[current_index])
        {
            ++current_index;
            return evaluate_path(*x);
        }
    }
}

json_value json_path::evaluate_path(json_object &object)
{
    auto value = object[path[current_index]];
    if (current_index + 1 < path.size() && std::get<2>(value) == nullptr)
    {
        throw "Invalid query";
    }
    if (current_index + 1 == path.size())
    {
        current_index = 0;
        return value;
    }
    ++current_index;
    return this -> evaluate_path(*(std::get<2>(value)));
}
