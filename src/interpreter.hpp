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
    std::shared_ptr<resp_or_syscall> evaluate(global_context &context, std::string const &input);
    void handle_history_run(global_context &context, std::string const &number);
    std::shared_ptr<resp_or_syscall> evaluate_response(global_context &context, std::string const &input);
    std::shared_ptr<resp_or_syscall> get_special_input(global_context &context, std::string const &input);
    std::shared_ptr<std::string> format_grammar_lines();
    std::shared_ptr<std::string> format_history_for_string();
    std::shared_ptr<std::string> format_objects(global_context const &context);
    std::shared_ptr<std::string> try_to_find_json_access(global_context &context, std::string const &input);
public:
    int loop_iteration(global_context &context, std::string const &shell_name);
};

void main_loop(global_context &context, std::shared_ptr<repl_options> const &opts);

#endif /* interpreter_hpp */
