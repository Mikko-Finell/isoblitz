#include "repl.hpp"
#include <iostream>
#include <string>
#include <cassert>

static s7_pointer main_quit(s7_scheme * sc, s7_pointer args);
static bool is_balanced(const std::string & str);
static bool is_not_white(const std::string & str);

s7_scheme * const REPL::init() {
    s7 = s7_init();
    return s7;
}

void REPL::run() {
    assert(s7 != nullptr);
    s7_pointer val;
    std::string buffer;

    try {
        while (std::cin) {
            std::cout << "\ns7> ";
            buffer = "";
            while (true) {
                std::string line;
                std::getline(std::cin, line);

                buffer = buffer + line + "\n";
                if (is_balanced(buffer)) {
                    break;
                }
            }
            if (is_not_white(buffer)) {
                val = s7_eval_c_string(s7, buffer.c_str());
                std::cout << s7_object_to_c_string(s7, val);
            }
        }
    }
    catch(...) {
    }
}

s7_scheme * const REPL::get() {
    return s7;
}

void REPL::eval(const std::string & sexp) {
    s7_pointer val = s7_eval_c_string(s7, sexp.c_str());
    std::cout << s7_object_to_c_string(s7, val);
}

static s7_pointer main_quit(s7_scheme * sc, s7_pointer args) {
    throw 0;
    return s7_nil(sc);
}

static bool is_balanced(const std::string & str) {
    int parens = 0;
    int quotes = 0;
    unsigned i = 0;
    while (i < str.size()) {
        if (str[i] == ';') {
            for (i = i + 1; i < str.size(); i++) {
                if (str[i] == '\n') {
                    break;
                }
            }
        }
        else if (str[i] == '"') {
            if (i == 0 || str[i - 1] != '\\') {
                quotes = 1;
                for (i = i + 1; i < str.size(); i++) {
                    if (str[i] == '"' && str[i - 1] != '\\') {
                        quotes = 0;
                        break;
                    }
                }
                if (quotes) {
                    return false;
                }
            }
        }
        else if (str[i] == '(') {
            parens++;
        }
        else if (str[i] == ')') {
            parens--;
        }
        i++;
    }
    return (parens == 0) && (quotes == 0);
}

static bool is_not_white(const std::string & str) {
    for (unsigned i = 0; (i < str.size() && str[i] != ';'); i++) {
        if (str[i] != ' ' && str[i] != '\n' && str[i] != '\t') {
            return true;
        }
    }
    return false;
}

/* g++ -I. -c repl.cpp
 * g++ -o repl repl.o s7.o -ldl
 */
