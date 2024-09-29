
#include "Application.h"

#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"
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

#include <../Dependencies/Freetype_Font/include/ft2build.h>

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

    FModAudio Application::audio;
    
    logging::Logger p;
    FModAudio Application::audio2;

    // Audio Demo timer
    float audioTimer = 3.0f;
    bool audio2_bool = true;

    float LastTime = glfwGetTime();;


    int Application::Init() {
        /*--------------------------------------------------------------
        INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS("Debugging/LogFile.txt");
        LOGGING_INFO("Application Start");
        p.m_Setup_Abort_Handler();
        std::signal(SIGABRT, logging::Logger::m_Abort_Handler);

        /*--------------------------------------------------------------
          INITIALIZE WINDOW WIDTH & HEIGHT
       --------------------------------------------------------------*/
        Serialization::Serialize::LoadConfig();
        LOGGING_INFO("Load Config Successful");

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        AstManager = AssetManager::funcGetInstance();
        AstManager->funcLoadAssets();
        LOGGING_INFO("Load Asset Successful");


        /*--------------------------------------------------------------
            INITIALIZE AUDIO MANAGER
        --------------------------------------------------------------*/
        // Initialize the FMOD system
        audio.init();
        audio2.init();
        audio.createSound("Assets/vacuum.mp3");
        audio2.createSound("Assets/zwing.wav");


        LOGGING_INFO("Application Init Successful");

       /*--------------------------------------------------------------
        INITIALIZE Asset Manager
       --------------------------------------------------------------*/
        AstManager->testJSON();

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        pipe = GraphicsPipe::funcGetInstance();
        pipe->funcInit();
        LOGGING_INFO("Load Graphic Pipline Successful");

        /*--------------------------------------------------------------
           INITIALIZE Input
        --------------------------------------------------------------*/
        //call back must happen before imgui
        Input.SetCallBack(lvWindow.Window);
        LOGGING_INFO("Set Input Call Back Successful");

        /*--------------------------------------------------------------
           INITIALIZE IMGUI
        --------------------------------------------------------------*/
        const char* glsl_version = "#version 130";
        imgui_manager.Initialize(lvWindow.Window, glsl_version);
        LOGGING_INFO("Load ImGui Successful");
        
        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        Ecs::ECS* ecs = Ecs::ECS::GetInstance();
        ecs->Load();
        ecs->Init();
        LOGGING_INFO("Load ECS Successful");

        LOGGING_INFO("Application Init Successful");
    
        return 0;
	}



    int Application::Run() {
        Ecs::ECS* ecs = Ecs::ECS::GetInstance();
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        float FPSCapTime = 1.f / help->FpsCap;
        double lastFrameTime = glfwGetTime();
        /*--------------------------------------------------------------
            GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.Window))
        {
            try {
                /* Poll for and process events */
                glfwPollEvents();

                /*--------------------------------------------------------------
                    UPDATE ECS
                    --------------------------------------------------------------*/
                ecs->Update(Helper::Helpers::GetInstance()->DeltaTime);

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
                    Calculate time
                    --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                help->DeltaTime = static_cast<float>(currentFrameTime - lastFrameTime);

                while (help->DeltaTime < FPSCapTime) {
                    lastFrameTime = currentFrameTime;
                    currentFrameTime = glfwGetTime();
                    help->DeltaTime += static_cast<float>(currentFrameTime - lastFrameTime);
                }
                lastFrameTime = glfwGetTime();
                help->Fps = 1.f / help->DeltaTime;

                glfwSwapBuffers(lvWindow.Window);
            }
            catch (const std::exception& e) {
                LOGGING_ERROR("Exception in game loop: {}", e.what());
            }
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
        audio2.shutdown();

        return 0;
	}

}