#include "./logger.h"

namespace WindEngine {

    Log::Config Log::c;
    std::ofstream Log::file;
    tm Log::timeInfo;
    int Log::numSection = 0;

    //============================================================//

    void Log::close() {
        Log::info() << "Completion of program execution";
        file.close();
    }

    //============================================================//

    void Log::config(void (*config)(Config& self)) {
        config(Log::c);

        time_t t = time(NULL);
        Log::timeInfo = *localtime(&t);

        if (std::filesystem::exists(Log::c.folderToSave)) {
            int min = INT16_MAX, max = 0;
            int numFiles = 0;
            for (auto f : std::filesystem::directory_iterator(Log::c.folderToSave)) {
                std::string path = f.path().string();
                if (path.substr(path.size() - 4, path.size()) == ".log") {
                    int i = atoi(f.path().filename().string().substr(0, path.size() - 4).c_str());

                    if (i < min) min = i;
                    if (i > max) max = i;

                    numFiles++;
                }
            }

            if (numFiles >= 10) {
                remove((
                        Log::c.folderToSave +
                        std::to_string(min) + ".log"
                    ).c_str()
                );
            }

            std::string path =  Log::c.folderToSave +
                                std::to_string(max + 1) + ".log";
            Log::file.open(path.c_str());

            if (!file.is_open())
                Log::warning() << "File for save log does not exist:" << path;
        } else
            Log::warning() << "Directory for save log files does not exist:" << Log::c.folderToSave;

        atexit(Log::close);
        Log::info() << "Logger started";
    }

    Log::Message Log::write(std::string tag) {
        return Log::Message(tag);
    }

    Log::Message Log::info() {
        return write("INFO");
    }

    Log::Message Log::debug() {
        return write("DEBUG");
    }

    Log::Message Log::warning() {
        return write("WARNING");
    }

    Log::Message Log::error() {
        return write("ERROR");
    }


    Log::Message Log::begin() {
        return write("BEGIN");
    }

    Log::Message Log::end() {
        return write("END");
    }

    //============================================================//

    Log::Message::Message(const Message& copy) {
        this->last = true;
        this->message << copy.message.str();
        this->color = copy.color;
        this->tag = copy.tag;
    }

    Log::Message::Message(std::string tag) {
        this->tag = tag;
        this->last = true;

        if (c.colors.find(tag) != c.colors.end())
            this->color = c.colors[tag];
        else if (tag == "BEGIN" || tag == "END")
            this->color = c.colors["SECTION"];
        else this->color = 7;
    }

    Log::Message::~Message() {
        if (!last) return;

        bool begin = (tag == "BEGIN");

        Log::numSection -= (tag == "END");
        Log::numSection = Log::numSection < 0 ? 0 : Log::numSection;

        if (tag == "BEGIN" || tag == "END")
            tag = "SECTION";

        if (!c.filters.empty() && std::find(c.filters.begin(), c.filters.end(), tag) == c.filters.end() && tag != "SECTION")
            return;

        std::string offset = std::string(Log::numSection * Log::c.numSpace, ' ');

        std::string data = offset + "[" + std::to_string(clock()) + "] " + "{" + tag + "} ";

        std::stringstream ss;
        std::string str = message.str();
        for (size_t i = 0; i < str.size(); ++i)
            if (str[i] == '\n')
                str.insert(i + 1, std::string(Log::numSection * Log::c.numSpace + data.size(), ' '));

        ss << data << str << "\n";

        if (Log::c.outConsole) {
            std::string txtColor =  "\033[1;3" + std::to_string(color) + "m";
            std::cout << (txtColor + ss.str() + "\033[1;37m").c_str();
        }

        if (!Log::c.folderToSave.empty()) {
            Log::file << color << ss.str();
            Log::file.flush();
        }

        Log::numSection += begin;
    }
}
