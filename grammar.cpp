//
//  grammar.cpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include "grammar.hpp"
#include <fstream>

std::shared_ptr<grammar> load_grammar(std::string const &path)
{
	grammar *ret = new grammar;
	
	std::fstream cin(path);
	std::string s;
	
	while(getline(cin, s))
	{
		ret->add_line(s);
	}
	
	return std::shared_ptr<grammar>(ret);
}


void grammar::add_line(std::string const &line)
{
	lines->push_back(line);
}
