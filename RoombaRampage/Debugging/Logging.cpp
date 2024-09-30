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


namespace logging {

    Logger& Logger::m_GetInstance()
    {
        static Logger instance{};
        return instance;
    }
    // Constructor: Opens the log file in append mode
    Logger::Logger(const std::string& filename)
    {
        m_logFile.open(filename,std::ios::out | std::ios::trunc);
        //std::cout << filename << std::endl;
        if (!m_logFile.is_open()) {
            std::cerr << "Error opening log file." << std::endl;
        }

       

    }

    void Logger::m_Setup_Abort_Handler() {
        // Handle abort signals
        std::signal(SIGABRT, m_Abort_Handler);

        // Handle segmentation fault (invalid memory access)
        std::signal(SIGSEGV, m_Abort_Handler);

        // Handle floating point exceptions (divide by zero, etc.)
        std::signal(SIGFPE, m_Abort_Handler);

        // Handle illegal instruction exceptions
        std::signal(SIGILL, m_Abort_Handler);

        // Handle termination requests (can be caught for cleanup)
        std::signal(SIGTERM, m_Abort_Handler);
    }
    void Logger::m_Abort_Handler(int signal) {
        auto location = std::source_location::current();
        std::string signalDescription{};
        switch (signal) {
        case SIGABRT:
            signalDescription = "SIGABRT: Program aborted";
            break;
        case SIGSEGV:
            signalDescription = "SIGSEGV: Segmentation fault";
            break;
        case SIGFPE:
            signalDescription = "SIGFPE: Floating-point exception";
            break;
        case SIGILL:
            signalDescription = "SIGILL: Illegal instruction";
            break;
        case SIGTERM:
            signalDescription = "SIGTERM: Termination request";
            break;
        default:
            signalDescription = "Unknown signal";
            break;
        }
        LOGGING_CRASH("Abort Signal received: {}",
            signalDescription);

        // Capture the stack trace
        backward::StackTrace st;
        st.load_here(32);

        // Resolve the stack trace to get function, file, and line information
        backward::TraceResolver resolver;
        resolver.load_stacktrace(st);

        // Log each frame of the stack trace with detailed info
        for (size_t i = 0; i < st.size(); ++i) {
            backward::ResolvedTrace trace = resolver.resolve(st[i]);
            if (!trace.source.filename.empty()) {
                LOGGING_CRASH("Frame {}: Function: {} in file: {} at line: {}", i,
                    trace.source.function,
                    trace.source.filename,
                    trace.source.line);
            }
            else {
                LOGGING_CRASH("Frame {}: Function: {} (unknown source)", i, trace.source.function);
            }
        }

        // Initialize the logger and log stack trace
        auto& logger = logging::Logger::m_GetInstance();
        logger.m_printer.print(st, logger.m_logFile); // Using the printer in Logger to log the stack trace
    }

    void Logger::m_TestingLog() {
        LOGGING_INFO("Testing of Logging Information {}" , 50);
        LOGGING_DEBUG("Testing of Logging Debug");
        LOGGING_ERROR("Testing of Logging Error with Source Location");
        LOGGING_ERROR_NO_SOURCE_LOCATION("Testing of Logging without source location");        
    }
    // Destructor: Closes the log file
    Logger::~Logger() { m_logFile.close(); }

    void Logger::m_Init(const std::string& filename) {
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
            m_logFile.open(filename, std::ios::out | std::ios::trunc);
            if (!m_logFile.is_open()) {
                throw std::ios_base::failure("Failed to open file: " + filename);
            }
        }
        catch (const std::exception& e) {
            LOGGING_ERROR("Error Init Logging File {}" , e.what());
        }
    }


    std::string Logger::m_GetCurrentTimestamp() {
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

    std::vector<std::string> Logger::m_GetLogList() {
        return  m_log_list;
    }

    // Logs a message with a given log level
    void Logger::m_Log(LogLevel level, const std::string& message)
    {
        std::string current_Time = m_GetCurrentTimestamp();
        // Create log entry
        std::ostringstream logEntry;
        logEntry << "[" << current_Time << "] " << m_LevelToString(level) << ": " << message << "\n";

        // Output to console
        std::cout << m_ColorToString(level) << logEntry.str() << s_CLOSE << std::endl;
        m_log_list.push_back(logEntry.str());
        // Output to log file
        if (m_logFile.is_open()) {
            m_logFile << logEntry.str();
            m_logFile.flush(); // Ensure immediate write to file
        }
    }

    // Converts log level to a string for output
    std::string Logger::m_LevelToString(LogLevel level)
    {
        switch (level) {
        case LogLevel::LOG_DEBUG:
            return "DEBUG";
        case LogLevel::LOG_INFO:
            return "INFO";
        case LogLevel::LOG_WARNING:
            return "WARNING";
        case LogLevel::LOG_ERROR:
            return "ERROR";
        default:
            return "UNKNOWN";
        }
    }

    std::string Logger::m_ColorToString(LogLevel level) {
        switch (level) {
        case LogLevel::LOG_DEBUG: //GREEN
            return s_GREEN;
        case LogLevel::LOG_INFO:  //WHITE
            return s_WHITE;
        case LogLevel::LOG_WARNING: //YELLOW
            return s_YELLOW;
        case LogLevel::LOG_ERROR:   //RED
            return s_RED;
        default:
            return s_CLOSE;
        }
    }    
}
