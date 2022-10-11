#include "./testing_tokenizer.h"

namespace WindEngineTesting {
    class UNIT_TEST(TestingTokenizerTest)
        using Token = Tokenizer::Token;
    private:
        std::string m_dir;
        std::string m_path;

        std::vector<std::string> m_input;
        std::vector<Token> m_output;
    protected:
        bool onCreate() override {
            m_dir = "./tmp/";
            m_path = m_dir + "test";
            
            m_input = std::vector<std::string> {
                "\"--------------------\"",
                "This is ",
                "test.",
                "\"(-_-) + (+_+)\" = love",
                "09.10.2022",
                "By NeutrinoZh",
                "\"--------------------\"",
                ""
            };

            m_output = std::vector<Token> {
                Token("text", "--------------------"),
                Token("word", "This"),
                Token("word", "is"),
                Token("word", "test"),
                Token("operator", "."),
                Token("text", "(-_-) + (+_+)"),
                Token("operator", "="),
                Token("word", "love"),
                Token("number", "09"),
                Token("operator", "."),
                Token("number", "10"),
                Token("operator", "."),
                Token("number", "2022"),
                Token("word", "By"),
                Token("word", "NeutrinoZh"),
                Token("text", "--------------------"),
                Tokenizer::NIL
            };
            
            return true;
        }
        
        bool onInit() override {
            try {
                fs::create_directory(m_dir);
            } catch (fs::filesystem_error& ex) {
                Log::error() << "Couldn't create tmp directory for testing tokenizer";
                return false;
            }

            std::ofstream file;

            file.open(m_path);
            if (!file.is_open()) {
                Log::error() << "Couldn't create file for testing tokenizer";
                return false;
            }
            
            for (std::string& line : m_input)
                file << line << "\n";

            file.close();
            return true;
        }

        bool onCompleted() override {
            try {
                fs::remove_all(m_dir);
                return true;
            } catch (fs::filesystem_error& ex) {
                Log::error() << "Couldn't remove tmp directory for testing tokenizer";
                return false;
            }
        }

        bool onTest() override {
            TestingTokenizer tokenizer = TestingTokenizer(m_path);

            Token i_token;
            for (Token o_token : m_output) {
                i_token = tokenizer.getRelative();
                ASSERT(o_token == i_token, (
                    "received token: (" + i_token.m_type + "; \"" + i_token.m_value + "\"), " +
                    "but should have been: (" + o_token.m_type + "; \"" + o_token.m_value + "\")"
                ).c_str());
            }

            return true;
        }
    };
}