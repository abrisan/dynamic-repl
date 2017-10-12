#include "json_file.hpp"

#include <fstream>
#include <sstream>

void process_vector(global_context &context, std::vector<std::string> &strings)
{
    std::unique_ptr<std::ostringstream> s(new std::ostringstream);
    for (auto const &str : strings)
    {
        (*s) << str;
    }

    std::string string_json = s -> str();

    strings.clear();

    size_t length = string_json.size();

    s -> str("");

    for (size_t i = 0 ; i < length ; ++i)
    {
        if (string_json[i] == '{')
        {
            std::shared_ptr<json_object> ret = parse_json_string_joined(string_json.substr(i, string_json.size() - i));
            if (ret == nullptr)
            {
                return;
            }
            else
            {
                if (!s -> str().size())
                {
                    return;
                }
                ret -> set_name(s -> str());
                context . loaded_json_objects() . push_back(ret);
                context . get_json_names() . push_back(s -> str());
            }
            return;
        }
        else if (string_json[i] == '=' || string_json[i] == ' ')
        {
            continue;
        }
        else
        {
            (*s) << string_json[i];
        }
    }

}


void load_json_file(global_context &context, std::string path)
{
    std::ifstream cin(path);

    std::string s;
    std::vector<std::string> strings;

    while(std::getline(cin, s))
    {
        strings.push_back(s);
        if (s[s.size() - 1] == ';')
        {
            process_vector(context, strings);
        }
    }
    if (strings.size())
    {
        process_vector(context, strings);
    }
    cin.close();
}
