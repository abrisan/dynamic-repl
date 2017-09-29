//
//  interpreter.hpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#ifndef interpreter_hpp
#define interpreter_hpp

#include "options.hpp"

struct resp_or_syscall
{
    std::shared_ptr<std::string> response;
    int syscall;
    bool set;
};

class interpreter
{
private:    
    std::shared_ptr<resp_or_syscall> evaluate(std::string const &input);
    void handle_history_run(std::string const &number);
    std::shared_ptr<resp_or_syscall> evaluate_response(std::string const &input);
    std::shared_ptr<resp_or_syscall> get_special_input(std::string const &input);
    std::shared_ptr<std::string> format_grammar_lines();
    std::shared_ptr<std::string> format_history_for_string();
public:
    int loop_iteration(std::string const &shell_name);
};

void main_loop(std::shared_ptr<repl_options> const &opts);

#endif /* interpreter_hpp */
