#include "./testing_tokenizer.h"

namespace WindEngineTesting {
    bool TESTING_TOKENIZER_TEST_INIT() {
        try {
            fs::create_directory("./tmp/");
        } catch (fs::filesystem_error& ex) {
            Log::error() << "Couldn't create tmp directory for testing tokenizer";
            return false;
        }

        std::ofstream file;

        file.open("./tmp/test_0");
        if (!file.is_open()) {
            Log::error() << "Couldn't create file for testing tokenizer";
            return false;
        }
        
        const char* test_0[] {
            "\"--------------------\"",
            "This is ",
            "test.",
            "\"(-_-) + (+_+)\" = love",
            "09.10.2022",
            "By NeutrinoZh",
            "\"--------------------\"",
            ""
        };

        for (unsigned int i = 0; test_0[i][0] != '\0'; ++i)
            file << test_0[i] << "\n";

        file.close();
        return true;
    }

    bool TESTING_TOKENIZER_TEST_DESTROY() {
        try {
            fs::remove_all("./tmp/");
            return true;
        } catch (fs::filesystem_error& ex) {
            Log::error() << "Couldn't remove tmp directory for testing tokenizer";
            return false;
        }
    }

    TEST(TESTING_TOKENIZER_TEST,
        using Token = Tokenizer::Token;
        TestingTokenizer tokenizer = TestingTokenizer("./tmp/test_0");

        const unsigned int SIZE = 16;
        const Token output[SIZE + 1] = {
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

        Token curr = tokenizer.getRelative();
        unsigned int i = 0;
        while (curr != Tokenizer::NIL) {
            ASSERT(curr == output[i] && i < SIZE, (
                "received token (" + curr.m_type + ";'" + curr.m_value + "'), " +
                "but should have been (" + output[i].m_type + ";'" + output[i].m_value + "')"
            ).c_str());

            ++i;
            curr = tokenizer.getRelative();
        }
    )   
}