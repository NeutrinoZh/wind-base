#include "../../src/Test/test.h"

namespace WindEngineTesting {
    using namespace WindEngine;

    class TestingTokenizer : public Tokenizer {
    public:
        TestingTokenizer(const std::string path):
                Tokenizer(path) {
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

    const Tokenizer::Middleware TestingTokenizer::ignore = 
            [](ITokenizer* self) {
                while (self->indexOf(" \t", self->get()) != -1)
                    self->next();

                if (self->isEndedLine())
                    return Tokenizer::REQUEST_NEWLINE;

                return self->nextMiddleware();
            };

    const Tokenizer::Middleware TestingTokenizer::word =
        [](ITokenizer* self) {
            std::string value = "";
            while (isalpha(self->get()))
                value += self->consume();
            
            if (!value.empty()) {
                if (value == "const")   return Token("const");
                if (value == "link")    return Token("link");
                if (value == "as")      return Token("as");
                if (value == "inherit") return Token("inherit");

                return Token("word", value);
            }

            return self->nextMiddleware();
        };

    const Tokenizer::Middleware TestingTokenizer::number =
        [](ITokenizer* self) {
            std::string value = "";
            while (isdigit(self->get())) 
                value += self->consume();

            if (!value.empty())
                return Token("number", value);
            return self->nextMiddleware();
        };

    const Tokenizer::Middleware TestingTokenizer::operators =
        [](ITokenizer* self) {
            if (self->indexOf("+-*/.=%$", self->get()) != -1)
                return Token("operator", std::string(1, self->consume()));
            return self->nextMiddleware();
        };

    const Tokenizer::Middleware TestingTokenizer::text = 
        [](ITokenizer* self) {
            if (self->skipIf('"')) {
                std::string value = "";
                while (self->get() != '"') {
                    if (self->isEndedLine())
                        return self->error("string literal ending not found");

                    value += self->consume();
                }
                self->next();

                return Token("text", value);
            }

            return self->nextMiddleware();
        };
        
    const Tokenizer::Middleware TestingTokenizer::error = 
        [](ITokenizer* self) {
            return self->error("unknown char");
        };
}