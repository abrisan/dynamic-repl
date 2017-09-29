//
//  globals.hpp
//  TripSLang
//
//  Created by Livecode on 9/28/17.
//  Copyright © 2017 alexbrisan. All rights reserved.
//

#ifndef GLOBALS
#define GLOBALS

#include <stdio.h>
#include <vector>
#include <string>
#include <stack>
#include "grammar.hpp"
#include <memory>

#define EXIT_REPL "exit"
#define REPL_HISTORY "history"
#define REPL_CLEAR_HISTORY "clearhist"
#define REPL_LOAD_GRAMMAR "load_grammar"

#define EXIT_REPL_SYSCALL 1
#define SHOW_REPL_HISTORY -1
#define NORM_SYSCALL 0

static std::vector<std::string> SPECIAL_STRINGS({
    EXIT_REPL,
    REPL_HISTORY,
    REPL_CLEAR_HISTORY,
        REPL_LOAD_GRAMMAR
});


typedef int PARSE_RESULT;

#define SUCC 0
#define FAIL 1

#define REPL_NAME "repl_name"

static std::vector<std::string> const CONFIG_OPTIONS({
    REPL_NAME
});

static std::vector<std::string> HISTORY;
static std::stack<std::shared_ptr<grammar>> GRAMMARS;

std::unique_ptr<std::vector<std::string>> split(std::string const &str, char const *delim);

#endif /* globals_hpp */
