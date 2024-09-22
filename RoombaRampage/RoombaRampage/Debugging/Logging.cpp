#include "Logging.h"

namespace Logging {
    // Constructor: Opens the log file in append mode
    Logger::Logger(const std::string& filename)
    {
        logFile.open(filename, std::ios::app);
        if (!logFile.is_open()) {
            std::cerr << "Error opening log file." << std::endl;
        }
    }

    // Destructor: Closes the log file
    Logger::~Logger() { logFile.close(); }

    std::string Logger::getCurrentTimestamp() {
        // Get current timestamp
        auto now = std::chrono::system_clock::now();

        // Convert to time_t to manipulate as a calendar time
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        // Convert to tm for local time formatting
        std::tm local_time;
        localtime_s(&local_time, &now_time);

        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_time);
        
        return std::string(buffer);
    }

    // Logs a message with a given log level
    void Logger::log(LogLevel level, const std::string& message)
    {
        std::string current_Time = getCurrentTimestamp();
        // Create log entry
        std::ostringstream logEntry;
        logEntry << "[" << current_Time << "] " << levelToString(level) << ": " << message << std::endl;

        // Output to console
        std::cout << logEntry.str();

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str();
            logFile.flush(); // Ensure immediate write to file
        }
    }

    // Converts log level to a string for output
    std::string Logger::levelToString(LogLevel level)
    {
        switch (level) {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        case CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
}
