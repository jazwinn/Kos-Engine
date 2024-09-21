#include <iostream>
#include "fmod.hpp"
#include "Application.h"

    int main(void)
    {

        // Enable run-time memory check for debug builds.
        #if defined(DEBUG) | defined(_DEBUG)
                _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
        #endif

        Application::Application::Init();

        FMOD_RESULT result;
        FMOD::System* pSystem = nullptr;
        result = FMOD::System_Create(&pSystem);

        //pSystem->init(32, FMOD_INIT_NORMAL, nullptr);

        //FMOD::Sound* pSound = nullptr;
        //pSystem->createSound(R"(C:\Users\blare\OneDrive\Desktop\Gam 200 Roomba OS\RoombaRampage\RoombaRampage\vacuum-cleaner-77085.mp3)", FMOD_DEFAULT, nullptr, &pSound);

        //FMOD::Channel* pChannel = nullptr;
        //result = pSystem->playSound(pSound, nullptr, false, &pChannel);

        //if (result == FMOD_OK)
        //{
        //    std::cout << "Sound is Playing\n";
        //}

        //bool bIsPlaying = true;

        //while (bIsPlaying)
        //{
        //    pChannel->isPlaying(&bIsPlaying);
        //    pSystem->update();

        //}
        //std::cout << result << std::endl;

        Application::Application::Run();

        Application::Application::Cleanup();

        return 0;
    }