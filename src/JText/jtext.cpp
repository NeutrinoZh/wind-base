#include "./jtext.h"

namespace WindEngine {
    bool JText::value(std::string& out) {
        if (
            eqTokenType("NUMBER") ||
            eqTokenType("STRING")
        ) {
            out = consume().m_value;
            return true;
        }
        
        Log::error() << "error value";
        return false;
    }

    JObject* JText::variable() {
        if (eqTokenType("WORD")) {
            std::string var_name,
                        var_value;

            var_name = consume().m_value;     
            if (!value(var_value))
                return nullptr;
            
            return new JObject(var_name, var_value);
        }

        Log::error() << "error variable";
        return nullptr;
    }

    //===========================================================//

    bool JText::eqToken(std::string type, std::string value) {
        return get(0).m_value == value && eqTokenType(type);
    }

    bool JText::eqTokenType(std::string type) {
        return get(0).m_type == type;
    }

    Tokenizer::Token JText::consume() {
        m_tokenizer.getRelative();
        return m_tokenizer.getRelative(-1);
    }

    Tokenizer::Token JText::get(unsigned int relative_position) {
        return m_tokenizer.getAbsolute(m_position + relative_position);
    }
    
    Tokenizer::Token JText::next(unsigned int step) {
        return m_tokenizer.getAbsolute(m_position + step);
    }
}