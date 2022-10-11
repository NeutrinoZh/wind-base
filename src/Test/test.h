#ifndef WIND_TEST_H
#define WIND_TEST_H

#include "../JText/jtext_tokenizer.h"

namespace WindEngine {
    #define ASSERT(exp, msg) if (_assert_((exp), __FILE__, __LINE__, (msg))) return false
    bool _assert_(bool exp, const char* file, const int line, const char* msg) {
        if (exp) return false;
        Log::error() << file << "(" << line << "): " << msg;
        return true;
    }

    #define UNIT_TEST(_name_) _name_ : public UnitTest { \
        public: _name_(): UnitTest(#_name_) {} private:

    class UnitTest {
    private:
        std::string m_name;
    public:
        std::string name() {
            return m_name;
        }

        void run() {
            Log::info() << "Run test: " + m_name;

            if (!onCreate()) {
                Log::error() << "Couldn't create test";
                return;
            }

            if (!onInit()) {
                Log::error() << "Couldn't init test";
                return;
            }

            if (!onTest()) {
                Log::error() << "Test failed";
                return;
            }

            if (!onCompleted()) {
                Log::error() << "Couldn't completed test";
            }

            Log::info() << "Test completed";
        }
    protected:
        UnitTest(const char* name) {
            m_name = name;
        };

        virtual bool onCreate() = 0;
        virtual bool onInit() = 0;
        virtual bool onCompleted() = 0;
        virtual bool onTest() = 0;
    };
}

#endif // WIND_TEST_H