#include <iostream>
#include "fmod.hpp"
#include "Application.h"
#include "../Logging.h"

    int main(void)
    {
        
        // Enable run-time memory check for debug builds.
        #if defined(DEBUG) | defined(_DEBUG)
                _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        #endif
        Application::Application app{};

        LOGGING_INIT_LOGS("LogFile.txt");
        LOGGING_ERROR("FAILED");
        LOGGING_ERROR_NO_SOURCE_LOCATION("OOPS");
        LOGGING_WARN("ALAMAK {}", 50);
        LOGGING_INFO("RIP");
        LOGGING_DEBUG("TESTING");

        app.Init();

        app.Run();

        app.Cleanup();
        return 0;
    } 