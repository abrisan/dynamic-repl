//
//  json_repl.hpp
//  dynamic-repl
//
//  Created by Alexandru Brisan on 04/10/2017.
//  Copyright © 2017 Alexandru Brisan. All rights reserved.
//

#ifndef json_repl_hpp
#define json_repl_hpp

#include <string>

#ifndef JSON_SYSCALLS
#define JSON_SYSCALLS
#define NEW_OBJECT "@new"
#define RELEASE_OBJECT "@delete"
#define SHOW_OBJECTS "@ls"
#define QUIT_JSON "@quit_json_mode"
#define JSON_INVALID_SYSCALL -1
#define JSON_NORM_SYSCALL 0
#define JSON_EXIT_SYSCALL 1
#endif

#include "globals.hpp"

void run_json_repl(global_context &context);

#endif /* json_repl_hpp */
