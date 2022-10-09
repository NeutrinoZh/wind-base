#ifndef JTEXT_H
#define JTEXT_H

#include "./jtext_tokenizer.h"

namespace WindEngine {
    class JObject {
    public:
        std::string name, value;
        std::vector<JObject*> children;

        JObject(std::string name, std::string value):
            name(name), value(value) {}
    };

    class JText {
    private:
        JTextTokenizer m_tokenizer;
        unsigned int m_position = 0;

        using Token = Tokenizer::Token;
    public:
        static bool parse(const std::string path, JObject& out);
    private:
        bool value(std::string& result);

        JObject* variable();
    private:
        Token get(unsigned int relative_position = 0);
        Token next(unsigned int step = 0);
        Token consume();

        bool eqToken(std::string type, std::string value);
        bool eqTokenType(std::string type);
    };
}

#endif