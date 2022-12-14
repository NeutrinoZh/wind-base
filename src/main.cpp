#define WIND_TEST

//#include "./Test/test.h"
#include "../tests/test_main.h"

namespace wd = WindEngine;

int main() {
    using namespace wd;

    Log::config([](auto& self) {
        self.outConsole = true;
        self.folderToSave = "./asset/logs/";
        self.numSpace = 2;
    });


#ifndef WIND_TEST
    JTextTokenizer tokenizer("./asset/file.jt");
    if (!tokenizer.is_open()) {
        Log::error() << "Could open file for tokenization";
        return EXIT_SUCCESS;
    }

    auto token = tokenizer.getRelative();
    while (token != Tokenizer::NIL) {
        Log::info() << token.m_type << ": \"" << token.m_value << "\"";
        token = tokenizer.getRelative();
    } 

    return EXIT_SUCCESS;
#else
    WindEngineTesting::test_main();
#endif
}