#ifndef __REPL__
#define __REPL__

#include "s7.h"
#include <string>

class REPL {
    s7_scheme * s7 = nullptr;

public:
    s7_scheme * const init();
    s7_scheme * const get();
    void run();

    template<typename... Args>
    void bind(Args... args) {
        s7_define_function(s7, args...);
    }

    void eval(const std::string & sexp);
};

#endif
