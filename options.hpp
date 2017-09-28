//
//  options.hpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#ifndef OPTIONS
#define OPTIONS

#include <unordered_map>
#include "globals.hpp"
#include <memory>

class repl_options
{
private:
	std::unique_ptr<std::unordered_map<std::string, std::string>> selected_options;
    PARSE_RESULT process_option(std::string const &s);
public:
	repl_options() : selected_options(new std::unordered_map<std::string, std::string>) {}
    PARSE_RESULT parse_file(std::string relative_path);
    std::string get_option_with_key(std::string const &name) const;
};



#endif /* options_hpp */
