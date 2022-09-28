#include "jtext_tokenizer.h"

namespace WindEngine {
    const Tokenizer::Middleware JTextTokenizer::ignore = 
        [](ITokenizer* self) {
            while (self->indexOf(" \t", self->get()) != -1)
                self->next();

            if (self->isEndedLine())
                return Tokenizer::REQUEST_NEWLINE;

            return self->nextMiddleware();
        };

    const Tokenizer::Middleware JTextTokenizer::word =
        [](ITokenizer* self) {
            std::string value = "";
            while (isalnum(self->get()))
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

    const Tokenizer::Middleware JTextTokenizer::number =
        [](ITokenizer* self) {
            std::string value = "";
            while (isdigit(self->get())) 
                value += self->consume();

            if (!value.empty())
                return Token("number", value);
            return self->nextMiddleware();
        };

    const Tokenizer::Middleware JTextTokenizer::operators =
        [](ITokenizer* self) {
            if (self->indexOf("{}()[]=,%", self->get()) != -1)
                return Token("operator", std::string(1, self->consume()));
            return self->nextMiddleware();
        };

    const Tokenizer::Middleware JTextTokenizer::text = 
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

    const Tokenizer::Middleware JTextTokenizer::error = 
        [](ITokenizer* self) {
            return self->error("unkown char");
        };
}