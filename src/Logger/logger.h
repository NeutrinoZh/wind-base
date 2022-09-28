#ifndef WIND_LOGGER_H_INCLUDED
#define WIND_LOGGER_H_INCLUDED

#include "../utils.h"

namespace WindEngine {
    class Log {
    private:

        struct Config {
            bool outConsole = false;
            std::string folderToSave = "";
            int numSpace = 1;

            std::map<std::string, uint8_t> colors = {
                { "ERROR", 1 },
                { "SECTION", 2 },
                { "WARNING", 3 },
                { "DEBUG", 4 },
                { "INFO", 7 },
            };

            std::vector<std::string> filters = {};
        };

    private:

        static Config c;

        static std::ofstream file;
        static tm timeInfo;
        static int numSection;

        static void close();

    public:

        static void config(void (*config)(Config& self));

    public:

        class Message {
        private:
            bool last;
            std::stringstream message;

            std::string tag;
            uint8_t color;
        public:
            ~Message();
            Message(const Message& copy);
            Message(std::string tag);

            template <typename T>
            Message operator<<(T value) {
                this->last = false;
                this->message << value;
                return *this;
            }

            Message operator<<(bool value) {
                this->last = false;
                this->message << (value ? "true" : "false");
                return *this;
            }
        };

    public:

        static Message write(std::string tag);

        static Message info();
        static Message debug();
        static Message warning();
        static Message error();

        static Message begin();
        static Message end();

    };
}

#endif // WIND_LOGGER_H_INCLUDED
