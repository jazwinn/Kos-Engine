#include <iostream>

#include "Application.h"

    int main(void)
    {

        // Enable run-time memory check for debug builds.
        #if defined(DEBUG) | defined(_DEBUG)
                _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        #endif

        Application::Application::Init();

        Application::Application::Run();

        Application::Application::Cleanup();

        return 0;
    }