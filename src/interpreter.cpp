//
//  interpreter.cpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#include "interpreter.hpp"
#include "globals.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <tuple>
#include "json_repl.hpp"
#include "json_path.hpp"



bool is_special_input(std::string const &input)
{
    for (auto const &s : SPECIAL_STRINGS)
    {
        if (s == input)
            return true;
    }
    return false;
}

std::shared_ptr<std::string> interpreter::format_history_for_string()
{
    if (HISTORY.empty())
            return std::shared_ptr<std::string>(new std::string(""));

    std::string *ret = new std::string;

    int i = 0;

    for (auto const &s : HISTORY)
    {
        ++i;
        ret -> append("\n");
        ret -> append(std::to_string(i) + ". ");
        if (s == "")
        {
            ret -> append("(implicit history)");
        }
        else
        {
            ret -> append(s);
        }
    }

    return std::shared_ptr<std::string>(ret);
}

std::shared_ptr<std::string> interpreter::format_grammar_lines()
{
    std::string *ret = new std::string("LOADED GRAMMAR:\n");
    auto lines = GRAMMARS.top()->get_lines();

    for (auto const &x: (*lines))
    {
        ret -> append(x);
        ret -> append("\n");
    }

    return std::shared_ptr<std::string>(ret);
}

std::shared_ptr<resp_or_syscall> interpreter::get_special_input(global_context &context, std::string const &input)
{
    if (input.length() == 0)
            return nullptr;
    std::unique_ptr<std::vector<std::string>> command(split(input, " "));
    std::string command_keyword = (*command)[0];

    if (!is_special_input(command_keyword))
        return nullptr;

    std::shared_ptr<resp_or_syscall> ret(new resp_or_syscall);

    if (command_keyword == EXIT_REPL)
    {
        ret->response = nullptr;
        ret->syscall = EXIT_REPL_SYSCALL;
    }
    else if(command_keyword == REPL_HISTORY)
    {
        ret->response = format_history_for_string();
        ret->syscall = NORM_SYSCALL;
    }
    else if (command_keyword == REPL_CLEAR_HISTORY)
    {
        HISTORY.clear();
        ret->response = std::shared_ptr<std::string>(new std::string(""));
        ret->syscall = NORM_SYSCALL;
    }
    else if(command_keyword == REPL_LOAD_GRAMMAR)
    {
        GRAMMARS.push(load_grammar((*command)[1]));
        HISTORY.push_back(input);
        ret->response = format_grammar_lines();
        ret->syscall = NORM_SYSCALL;
    }
    else if(command_keyword == REPL_RUN_HIST)
    {
        HISTORY.push_back(input);
        handle_history_run(context, (*command)[1]);
        ret -> response =  nullptr;
        ret -> syscall = RUN_REPL_HISTORY_SYSCALL;
    }
    else if (command_keyword == REPL_JSON)
    {
        HISTORY.push_back(input);
        run_json_repl(context);
        ret -> response = std::shared_ptr<std::string>(new std::string(""));
        ret -> syscall = NORM_SYSCALL;
    }
    return ret;
}

std::shared_ptr<std::string> unpack_json_value(json_value const &value)
{
    std::ostringstream output;
    auto string_try = std::get<0>(value);
    if (string_try != nullptr)
    {
        output << *string_try;
        output << " (std::string)";
        return std::shared_ptr<std::string>(new std::string(output.str()));
    }

    auto int_try = std::get<1>(value);
    if (int_try != nullptr)
    {
        output << *int_try;
        output << " (int)";
        return std::shared_ptr<std::string>(new std::string(output.str()));
    }

    auto json_object_try = std::get<2>(value);
    if (json_object_try != nullptr)
    {
        output << json_object_try -> to_string();
        output << " (json_object)";
        return std::shared_ptr<std::string>(new std::string(output.str()));
    }

    return std::shared_ptr<std::string>(new std::string("Object does not have attribute"));
}

std::shared_ptr<std::string> interpreter::try_to_find_json_access(global_context &context, std::string const &input)
{
    json_path path;
    path.parse_json_path(input);
    return unpack_json_value(path.evaluate_path(context));
}

std::shared_ptr<resp_or_syscall> interpreter::evaluate_response(global_context &context, std::string const &input)
{
    resp_or_syscall *ret = new resp_or_syscall;
    ret -> syscall = NORM_SYSCALL;
    if (input.length() == 0)
    {
        ret -> response = format_history_for_string();
    }
    else
    {
        auto could_be_json_command = try_to_find_json_access(context, input);
        if (could_be_json_command != nullptr)
        {
            ret -> response = could_be_json_command;
        }
        else
        {
            ret -> response = std::shared_ptr<std::string>(new std::string(input));
        }

    }
    return std::shared_ptr<resp_or_syscall>(ret);
}


void interpreter::handle_history_run(global_context &context, std::string const &number)
{
    size_t com_numb = atoi(number.c_str());
    if (com_numb >= HISTORY.size())
    {
        return;
    }
    auto command = HISTORY[com_numb-1];
    auto resp = evaluate(context, command);
    if (resp -> syscall == NORM_SYSCALL)
        std::cout << *(resp -> response) << std::endl << std::endl;
}

std::shared_ptr<resp_or_syscall> interpreter::evaluate(global_context &context, std::string const &input)
{

    auto special_input = get_special_input(context, input);
    if (special_input != nullptr)
    {
        return std::shared_ptr<resp_or_syscall>(special_input);
    }
    else
    {
        auto evaluated_response = evaluate_response(context, input);
        if (evaluated_response == nullptr)
        {
            throw "Unexpected nullptr expected";
        }
        HISTORY.push_back(input);
        return std::shared_ptr<resp_or_syscall>(evaluated_response);
    }
    return nullptr;
}

int interpreter::loop_iteration(global_context &context, std::string const &shell_name)
{
    std::string s;
    std::cout << shell_name << "> ";
    std::getline(std::cin, s);
    auto resp = evaluate(context, s);
    if (resp -> syscall == EXIT_REPL_SYSCALL)
    {
        return EXIT_REPL_SYSCALL;
    }
    else if (resp -> syscall == NORM_SYSCALL)
        std::cout << *(resp -> response) << std::endl << std::endl;
    return NORM_SYSCALL;
}

void main_loop(global_context &context, std::shared_ptr<repl_options> const &opts)
{
    interpreter t;
    std::string name = opts->get_option_with_key(REPL_NAME);
    while(true)
    {
        int exit_code = t.loop_iteration(context, name);
        switch(exit_code)
        {
            case NORM_SYSCALL:
                continue;
            case EXIT_REPL_SYSCALL:
                return;
        }
    }
}
