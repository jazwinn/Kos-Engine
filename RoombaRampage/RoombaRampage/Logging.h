#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <iomanip>


namespace Logging {
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL,
        LOGLEVEL_SIZE
    };

    class Logger {
    public:
        // Constructor: Opens the log file in append mode
        Logger(const std::string& filename);

        // Destructor: Closes the log file
        ~Logger();

        // Logs a message with a given log level
        void log(LogLevel level, const std::string& message);
        std::string getCurrentTimestamp();

    private:
        std::ofstream logFile; // File stream for the log file

        // Converts log level to a string for output
        std::string levelToString(LogLevel level);
    };
}

/**************************************************

HOW TO USE?

Logging::Logger log(<insert file name>)
log.log(<enum>, <text>);

and you are done

************************************************/

#endif
