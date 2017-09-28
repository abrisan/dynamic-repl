//
//  grammar.hpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#ifndef grammar_hpp
#define grammar_hpp

#include <string>
#include <vector>
#include <ostream>
#include <memory>

class grammar
{
private:
	std::shared_ptr<std::vector<std::string>> lines;
public:
	grammar() : lines(new std::vector<std::string>) {}
	void add_line(std::string const &path);
	std::shared_ptr<std::vector<std::string>> const get_lines() const {return this->lines;}
};

std::shared_ptr<grammar> load_grammar(std::string const &path);

inline std::ostream &operator<<(std::ostream &cout, grammar const &obj)
{
	cout << "== LOADED GRAMMAR IS ==\n";
	std::shared_ptr<std::vector<std::string>> const lines = obj.get_lines();
	for (auto const &x : (*lines))
	{
		cout << x << "\n";
	}
	return cout;
}

#endif /* grammar_hpp */
