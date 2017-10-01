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

int main()
{
    std::cout << "Please enter path to config file:\n";
    std::string s;
    std::cin >> s;
    
    std::shared_ptr<repl_options> options(new repl_options);
        
    options->parse_file(s);
    main_loop(options);
}
