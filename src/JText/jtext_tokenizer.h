#ifndef JTEXT_TOKENIZER_H
#define JTEXT_TOKENIZER_H

#include "../Tokenizer/tokenizer.h"

namespace WindEngine {
    class JTextTokenizer : public Tokenizer {
    public:
        JTextTokenizer(const std::string path): Tokenizer(path) {
            addMiddleware(ignore);
            addMiddleware(word);
            addMiddleware(number);
            addMiddleware(operators);
            addMiddleware(text);
            addMiddleware(error);
        }
    private:
        const static Middleware ignore;
        const static Middleware word;
        const static Middleware number;
        const static Middleware operators;
        const static Middleware text;
        const static Middleware error;
    };
}

#endif