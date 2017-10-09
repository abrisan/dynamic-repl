//
//  globals.hpp
//  TripSLang
//
//  Created by Alexandru Brisan on 9/28/17.
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
#include "json.hpp"
#include <tuple>

#define EXIT_REPL "exit"
#define REPL_HISTORY "history"
#define REPL_CLEAR_HISTORY "clearhist"
#define REPL_LOAD_GRAMMAR "load_grammar"
#define REPL_RUN_HIST ":run"
#define REPL_JSON ":json"
#define DEFAULT_PATH "/Users/alexandrubrisan/Desktop/dynamic-repl/dynamic-repl/options.config"

#define EXIT_REPL_SYSCALL 1
#define RUN_REPL_HISTORY_SYSCALL -1
#define NORM_SYSCALL 0

class global_context
{
private:
  std::vector<std::shared_ptr<json_object>> m_loaded_json_objects;
  std::vector<std::string> m_loaded_json_objects_name;
public:
  std::vector<std::shared_ptr<json_object>> const &loaded_json_objects() const {return m_loaded_json_objects;}
  std::vector<std::shared_ptr<json_object>> &loaded_json_objects() {return m_loaded_json_objects;}
  std::vector<std::string> const &get_json_names() const {return m_loaded_json_objects_name;}
  std::vector<std::string> &get_json_names() {return m_loaded_json_objects_name;}
};

static std::vector<std::string> SPECIAL_STRINGS({
    EXIT_REPL,
    REPL_HISTORY,
    REPL_CLEAR_HISTORY,
    REPL_LOAD_GRAMMAR,
    REPL_RUN_HIST,
    REPL_JSON
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


std::unique_ptr<std::vector<std::string>> split(std::string const &str, std::string const &delim);


#endif /* globals_hpp */
