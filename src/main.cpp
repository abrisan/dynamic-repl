//
//  main.cpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "interpreter.hpp"
#include "options.hpp"
#include "json.hpp"

void repl_main()
{
    std::cout << "Please enter path to config file:\n";
    std::string s;
    std::cin >> s;
    
    if (s == "def")
        s = DEFAULT_PATH;
    
    std::shared_ptr<repl_options> options(new repl_options);
    
    options->parse_file(s);
    main_loop(options);
}

void test_main()
{
    std::vector<std::string> *input = new std::vector<std::string>;
    input -> push_back("{name: \"Alex\", surname: \"Brisan\"");
    input -> push_back(", some_other_param: \"Hello, World!\", number_param: 1, obj: {some_obj:{num: 1}}}");
    auto obj = parse_json_string(std::shared_ptr<std::vector<std::string>>(input));
    json_value elem = (*obj)["name"];
    json_value elem2 = (*obj)["surname"];
    auto string_value = std::get<str_ptr>(elem);
    auto string_surname = std::get<str_ptr>(elem2);
    // std::cout << *string_value <<  " " <<  *string_surname << std::endl;
    std::cout << *obj << std::endl;
}
// Merge parser json pt string-uri


int main()
{
    std::cout << "Would you like a test main?";
    std::string s;
    std::cin >> s;
    if (s == "Yes")
        test_main();
    else
        repl_main();
    return 0;
}
