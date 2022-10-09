#ifndef WIND_TEST_H
#define WIND_TEST_H

#include "../JText/jtext_tokenizer.h"

namespace WindEngine {
    #define TEST(_name_, ...) bool _name_() { __VA_ARGS__ return true; }

    #define ASSERT(exp, msg) if (_assert_((exp), __FILE__, __LINE__, (msg))) return false
    bool _assert_(bool exp, const char* file, const int line, const char* msg) {
        if (exp) return false;
        Log::error() << file << "(" << line << "): " << msg;
        return true;
    }

    

    int test_main() {
        Log::begin() << "test_main called";



        Log::end() << "testing ended";
        return EXIT_SUCCESS;
    }
}

#endif // WIND_TEST_H