
#include "Application.h"

#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../Inputs/Input.h"
#include "../ECS/ECS.h"
#include "Helper.h"
#include "Window.h"
#include "../Debugging/Logging.h"
#include "../Debugging/Performance.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#include "../Assets/AudioManager.h"

namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    AppWindow Application::lvWindow;
    ImGuiHandler Application::imgui_manager;
    GraphicsPipe* pipe;
    AssetManager* AstManager;
    Input::InputSystem Input;
   

    // Audio
    FModAudio audio;
    FMOD_CHANNELGROUP* channelgroup;

    float LastTime = glfwGetTime();;


    int Application::Init() {
        /*--------------------------------------------------------------
          INITIALIZE WINDOW WIDTH & HEIGHT
       --------------------------------------------------------------*/
        Helper::Helpers::GetInstance()->WindowWidth = 1280;
        Helper::Helpers::GetInstance()->WindowHeight = 720;

        /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        AstManager = AssetManager::funcGetInstance();
        AstManager->funcLoadAssets();

        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        pipe = GraphicsPipe::funcGetInstance();
        pipe->funcInit();

        /*--------------------------------------------------------------
           INITIALIZE Input
        --------------------------------------------------------------*/
        //call back must happen before imgui
        Input.SetCallBack(lvWindow.Window);

        /*--------------------------------------------------------------
           INITIALIZE IMGUI
        --------------------------------------------------------------*/
        const char* glsl_version = "#version 130";
        imgui_manager.Initialize(lvWindow.Window, glsl_version);

        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        //fetch ecs
        Ecs::ECS* ecs = Ecs::ECS::GetInstance();

        ecs->Load();
        ecs->Init();

        /*--------------------------------------------------------------
            INITIALIZE AUDIO MANAGER
        --------------------------------------------------------------*/
        // Initialize the FMOD system
        audio.init();
        audio.createSound("vacuum.mp3");

        /*--------------------------------------------------------------
            INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS("LogFile.txt");
        LOGGING_INFO("Application Start");
        return 0;
	}



    int Application::Run() {

        Ecs::ECS* ecs = Ecs::ECS::GetInstance();
        float FPSCap = 1 / 60;



  

        /*--------------------------------------------------------------
         GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.Window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            //calculate DeltaTime
            float CurrentTime = static_cast<float>(glfwGetTime());
            float DeltaTime =  CurrentTime - LastTime;

            //std::cout << "FPS:" << 1/DeltaTime << std::endl;
            //PerformanceTracker::Performance a;
            //a.printFPS(DeltaTime);

            /*--------------------------------------------------------------
             UPDATE ECS
             --------------------------------------------------------------*/
            ecs->Update(DeltaTime);

            /*--------------------------------------------------------------
             UPDATE Render Pipeline
             --------------------------------------------------------------*/
            pipe->funcUpdate();

            /*--------------------------------------------------------------
             DRAWING/RENDERING Window
             --------------------------------------------------------------*/
            lvWindow.Draw();

            /*--------------------------------------------------------------
             DRAWING/RENDERING Objects
             --------------------------------------------------------------*/
            pipe->funcDrawWindow();

            /*--------------------------------------------------------------
             Draw IMGUI FRAME
             --------------------------------------------------------------*/          
            imgui_manager.Render();


            /*--------------------------------------------------------------
             Play AUDIO
             --------------------------------------------------------------*/
            //audio.playSound();

            glfwSwapBuffers(lvWindow.Window);

            while (DeltaTime < FPSCap) {
                CurrentTime = static_cast<float>(glfwGetTime());  // Continuously update current time
                DeltaTime = CurrentTime - LastTime;  // Calculate new DeltaTime
            }

            LastTime = CurrentTime;
        }

        return 0;
	}


	int Application::Cleanup() {

        Ecs::ECS::GetInstance()->Unload();
        imgui_manager.Shutdown();
        lvWindow.CleanUp();
        glfwTerminate();
        audio.shutdown();
        LOGGING_INFO("Application Closed");
        return 0;
	}

}