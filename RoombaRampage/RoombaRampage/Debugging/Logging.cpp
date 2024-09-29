/******************************************************************/
/*!
\file      Logging.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Logging functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Logging.h"


namespace Logging {

    Logger& Logger::GetInstance()
    {
        static Logger instance{};
        return instance;
    }
    // Constructor: Opens the log file in append mode
    Logger::Logger(const std::string& filename)
    {
        logFile.open(filename,std::ios::app);
        //std::cout << filename << std::endl;
        if (!logFile.is_open()) {
            std::cerr << "Error opening log file." << std::endl;
        }

       

    }
    void Logger::testingLog() {
        LOGGING_INFO("Testing of Logging Information {}" , 50);
        LOGGING_DEBUG("Testing of Logging Debug");
        LOGGING_ERROR("Testing of Logging Error with Source Location");
        LOGGING_ERROR_NO_SOURCE_LOCATION("Testing of Logging without source location");        
    }
    // Destructor: Closes the log file
    Logger::~Logger() { logFile.close(); }

    void Logger::Init(const std::string& filename) {
        assert(!m_bInitialized && "The logger must be initialized before it is used!");
        if (m_bInitialized)
        {
            std::cout << "The logger must be initialized before it is used!" << std::endl;
            return;
        }
        else {
            std::cout << "Logger initialized" << std::endl;
        }

        m_bInitialized = true;
        try {
            logFile.open(filename, std::ios::out | std::ios::app);
            if (!logFile.is_open()) {
                throw std::ios_base::failure("Failed to open file: " + filename);
            }
        }
        catch (const std::exception& e) {
            LOGGING_ERROR("Error Init Logging File {}" , e.what());
        }

        st.load_here(32);
        printer.print(st, logFile);
    }


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

    std::vector<std::string> Logger::getLogList() {
        return log_list;
    }

    // Logs a message with a given log level
    void Logger::Log(LogLevel level, const std::string& message)
    {
        std::string current_Time = getCurrentTimestamp();
        // Create log entry
        std::ostringstream logEntry;
        logEntry << "[" << current_Time << "] " << levelToString(level) << ": " << message << "\n";

        // Output to console
        std::cout << colorToString(level) << logEntry.str() << CLOSE << std::endl;
        log_list.push_back(logEntry.str());
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
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_INFO:
            return "INFO";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    std::string Logger::colorToString(LogLevel level) {
        switch (level) {
        case LOG_DEBUG: //GREEN
            return GREEN;
        case LOG_INFO:  //WHITE
            return WHITE;
        case LOG_WARNING: //YELLOW
            return YELLOW;
        case LOG_ERROR:   //RED
            return RED;
        default:
            return CLOSE;
        }
    }    
}
