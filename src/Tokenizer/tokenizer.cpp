#include "./tokenizer.h"

namespace WindEngine {
    const Tokenizer::Token Tokenizer::NIL = Tokenizer::Token("NIL", "NIL");
    const Tokenizer::Token Tokenizer::REQUEST_NEWLINE = Tokenizer::Token("\n", "\n");

    Tokenizer::Token Tokenizer::getAbsolute(unsigned int position) {
        if (!is_open())
            return Tokenizer::NIL;

        if (position < m_row) {
            return m_tokens[position];
        }

        if (m_row == m_curr_line.size()) {
            if (!getline(m_file, m_curr_line))
                return Tokenizer::NIL;

            m_row = 0;
            m_line += 1;
        }

        m_curr_middleware = 0;

        Token result = m_interface->nextMiddleware();
        if (result.m_type == "\n" || m_curr_line.empty())
            return getAbsolute(position + 1);

        m_tokens.push_back(result);

        return result;
    }

    Tokenizer::Token Tokenizer::getRelative(int relative_position) {
        unsigned int position = m_row + relative_position;
        if (position < 0 || position > m_curr_line.size()) {
            m_interface->error("when getting a token by a relative position, the position index is out of range");
            return Tokenizer::NIL;
        }

        return getAbsolute(position);
    }

    void Tokenizer::addMiddleware(Middleware software) {
        this->m_middleware.push_back(software);        
    }

    bool Tokenizer::is_open() {
        return m_file.is_open();
    }

    bool Tokenizer::open(const std::string path) {
        if (!m_interface)
            m_interface = new ITokenizer(*this);

        m_path = path;
        m_file.open(m_path.c_str());
        return m_file.is_open();
    }

    // =================== ITokenizer ========================

    Tokenizer::Token ITokenizer::nextMiddleware() {
        if (
            self.m_curr_middleware >= self.m_middleware.size() ||
            self.m_row >= self.m_curr_line.size()
        )
            return Tokenizer::NIL;
        return self.m_middleware[self.m_curr_middleware++](this);
    }

    int ITokenizer::indexOf(std::string str, char ch) { // std::find??
        unsigned int i;
        for (i = 0; i < str.size(); ++i)
            if (str[i] == ch) return i;
        return -1;
    }

    bool ITokenizer::isEndedLine() {
        return get() == '\0';
    }

    char ITokenizer::consume() {
        next();
        return get(-1);
    }

    char ITokenizer::get(unsigned int relative_position) {
        unsigned int position = self.m_row + relative_position;
        if (position >= self.m_curr_line.size())
            return '\0';
        return self.m_curr_line[position];
    }

    char ITokenizer::next(unsigned int step) {
        self.m_row += step;
        return this->get();
    }

    bool ITokenizer::skipIf(char ch) {
        if (get() == ch) {
            next();
            return true;
        }
    
        return false;
    }

    Tokenizer::Token ITokenizer::error(std::string message) {
        if (get() != '\0') {
            std::string pointer = std::string(self.m_curr_line.size(), ' ');
            pointer[self.m_row] = '^';
            Log::error() << self.m_path << "(" << self.m_line << ";" << self.m_row << "): " 
                        << message << " '" << get() << "':\n " << self.m_curr_line << "\n"
                        << " " << pointer;
        } else 
            Log::error() << self.m_path << "(" << self.m_line << ";" << self.m_row << "): " 
                        << message << "\n" << self.m_curr_line;

        return Tokenizer::NIL;
    }
}