#ifndef WIND_TOKENIZER_H_INCLUDED
#define WIND_TOKENIZER_H_INCLUDED

#include "../Logger/logger.h"

namespace WindEngine {
    class ITokenizer;

	class Tokenizer {
    protected:
        std::ifstream m_file;
        std::string m_path;

        std::string m_curr_line = "";
        std::string m_fullText = "";

        unsigned int m_line = 0, m_row = 0;
    public:
        struct Token {
            std::string m_value;
            std::string m_type;

            Token() = default;
            Token(std::string value) {
                m_type = m_value = value;
            }
            Token(std::string type, std::string value) {
                m_type = type;
                m_value = value;
            }

            bool operator==(Token token) {
                return (m_type == token.m_type && m_value == token.m_value);
            }

            bool operator!=(Token token) {
                return !(*this == token);
            }
        };

        const static Token NIL;

        Tokenizer() = default;
        Tokenizer(const std::string path) {
            open(path);
        }

        bool is_open();
        bool open(const std::string path);
        
        Token get();

        typedef std::function<Token(ITokenizer*)> Middleware;
    protected:    
        const static Token REQUEST_NEWLINE;

        std::vector<Middleware> m_middleware;

        void addMiddleware(Middleware);
    private:
        unsigned int m_curr_middleware = 0;
        ITokenizer* m_interface = NULL;

        friend class ITokenizer;
    };

    class ITokenizer {
    private:
        friend class Tokenizer;

        Tokenizer& self;
        ITokenizer(Tokenizer& tokenizer):
            self(tokenizer) {}
    public:
        bool isEndedLine();
        int indexOf(std::string str, char ch);

        char consume();
        char get(unsigned int relative_position = 0);
        char next(unsigned int step = 1);

        bool skipIf(char ch);

        Tokenizer::Token nextMiddleware();
        Tokenizer::Token error(std::string message);
    };
}

#endif