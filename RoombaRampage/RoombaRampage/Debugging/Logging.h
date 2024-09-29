#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>
#include <string_view>
#include <iomanip>
#include <cassert>
#include <source_location>
#include <format>

#include "../backward/backward.hpp"

/*
 * @brief Variadic Macro for logging Information. This macro takes in a string message, followed by the
 * necessary arguments.
 * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
 * the arguments
 */
#define LOGGING_INFO(x, ...) Logging::Logger::GetInstance().Info( x , __VA_ARGS__ )

 /*
  * @brief Variadic Macro for logging warnings. This macro takes in a string message, followed by the
  * necessary arguments.
  * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
  * the arguments
  */
#define LOGGING_WARN( x, ... ) Logging::Logger::GetInstance().Warn( x , __VA_ARGS__ )

  /*
   * @brief Variadic Macro for logging Errors. This macro takes in a string message, followed by the
   * necessary arguments.
   * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
   * the arguments
   */
#define LOGGING_ERROR(x, ...) Logging::Logger::GetInstance().Error(std::source_location::current(), x,  __VA_ARGS__)
#define LOGGING_ERROR_NO_SOURCE_LOCATION(x, ...) Logging::Logger::GetInstance().Error(x, __VA_ARGS__)
   /*
    * @brief Variadic Macro for logging Errors. This macro takes in a string message, followed by the
    * necessary arguments.
    * @param Takes an std::string_view or string in the form of "This is a log value: {0}, and {1}", followed by
    * the arguments
    */
#define LOGGING_DEBUG(x, ...) Logging::Logger::GetInstance().Debug(std::source_location::current(), x, __VA_ARGS__)


#define LOGGING_ASSERT(x) \
    if (!(x)) { \
        LOGGING_ERROR("Assertion failed: {}", x); \
        assert(x); \
    }

#define LOGGING_INIT_LOGS( filename ) Logging::Logger::GetInstance().Init( filename )

namespace Logging {

    //struct LogEntry {
    //    enum LogLevel {
    //        DEBUG,
    //        INFO,
    //        WARNING,
    //        ERROR,
    //        CRITICAL,
    //        LOGLEVEL_SIZE
    //    };
    //    LogLevel type{ LogLevel::INFO };
    //    std::string logMsg{ "" };
    //};

    static const std::string GREEN = "\033[0;32m";
    static const std::string YELLOW = "\033[0;33m";
    static const std::string RED = "\033[0;31m";
    static const std::string WHITE = "\033[0;30m";
    static const std::string BLUE = "\033[0;36m";
    static const std::string CLOSE = "\033[0m";

    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_LEVEL_SIZE
    };

    class Logger {
    public:
        //Default
        Logger() = default;

        // Constructor: Opens the log file in append mode
        Logger(const std::string& filename);

        // Destructor: Closes the log file
        ~Logger();

        void Init(const std::string& filename);
        static Logger& GetInstance();

        // Logs a message with a given log level
        void Log(LogLevel level, const std::string& message);

        template <typename... Args>
        void Info(const std::string_view message, Args&&... args);

        template <typename... Args>
        void Warn(const std::string_view message, Args&&... args);

        template <typename... Args>
        void Error(std::source_location location, const std::string_view message, Args&&... args);

        template <typename... Args>
        void Error(const std::string_view message, Args&&... args);

        template <typename... Args>
        void Debug(std::source_location location, const std::string_view message, Args&&... args);

        std::string getCurrentTimestamp();
        std::vector<std::string> getLogList();

    private:
        std::ofstream logFile; // File stream for the log file
        
        backward::Printer printer;// For printing crashes into the logfile
        backward::StackTrace st;

        bool m_bInitialized{ false };

        // Converts log level to a string for output
        std::string levelToString(LogLevel level);
        // add color to the text
        std::string colorToString(LogLevel level);
        std::vector<std::string> log_list;
    };


    template <typename... Args>
    void Logger::Info(const std::string_view message, Args&&... args)
    {
        assert(m_bInitialized && "The logger must be initialized before it is used!");

        if (!m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream logEntry;
        logEntry << "[INFO]: " << getCurrentTimestamp() << " - " << std::vformat(message, std::make_format_args(args...));
        std::cout << BLUE << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str() << "\n";
            logFile.flush(); // Ensure immediate write to file
        }
    }

    template <typename... Args>
    void Logger::Warn(const std::string_view message, Args&&... args)
    {
        assert(m_bInitialized && "The logger must be initialized before it is used!");

        if (!m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }
        std::stringstream logEntry;
        logEntry << "[WARN]: " << getCurrentTimestamp() << " - " << std::vformat(message, std::make_format_args(args...));
        std::cout << YELLOW << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str() << "\n";
            logFile.flush(); // Ensure immediate write to file
        }
        else {
            std::cout << "Not storing" << std::endl;
        }
    }
    template <typename... Args>
    void Logger::Error(std::source_location location, const std::string_view message, Args&&... args)
    {
        assert(m_bInitialized && "The logger must be initialized before it is used!");

        if (!m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream logEntry;
        logEntry << "[ERROR]: " << getCurrentTimestamp() << " - " << std::vformat(message, std::make_format_args(args...))
            << "\nFUNC: " << location.function_name() << " LINE: " << location.line() << " FILE: " << location.file_name();

        std::cout << RED << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str() << "\n";
            logFile.flush(); // Ensure immediate write to file
        }
        else {
            std::cout << "Not storing" << std::endl;
        }

    }

    template <typename... Args>
    void Logger::Error(const std::string_view message, Args&&... args)
    {
        assert(m_bInitialized && "The logger must be initialized before it is used!");

        if (!m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream logEntry;
        logEntry << "[ERROR]: " << getCurrentTimestamp() << " - " << std::vformat(message, std::make_format_args(args...));

        std::cout << RED << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str() << "\n";
            logFile.flush(); // Ensure immediate write to file
        }
    }

    template <typename... Args>
    void Logger::Debug(std::source_location location, const std::string_view message, Args&&... args)
    {
        assert(m_bInitialized && "The logger must be initialized before it is used!");

        if (!m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }

        std::stringstream logEntry;
        logEntry << "[DEBUG]: " << getCurrentTimestamp() << " - " << std::vformat(message, std::make_format_args(args...));

        std::cout << GREEN << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str() << "\n";
            logFile.flush(); // Ensure immediate write to file
        }

    }
}

/**************************************************

HOW TO USE?

Logging::Logger log(<insert file name>)
log.log(<enum>, <text>);

USING DEFINE
LOGGING_INIT_LOGS(<FILE PATH>);
LOGGING_INFO(<MESSAGE>, 50);
inside MESSAGE need to have {} as this is the placeholder for 50



************************************************/

#endif

