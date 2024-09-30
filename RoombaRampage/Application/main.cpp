#include <iostream>
#include "Application.h"
#include "../Debugging/Logging.h"

    int main(void)
    {
        
        // Enable run-time memory check for debug builds.
        #if defined(DEBUG) | defined(_DEBUG)
                _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        #endif


        Application::Application app{};        
       
        app.Init();
       

        app.Run();

        app.Cleanup();




        return 0;
    } 