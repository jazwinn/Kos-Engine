
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

#include "../Assets/Audio.h"

#include <../Dependencies/Freetype_Font/include/ft2build.h>

namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    AppWindow Application::lvWindow;
    ImGuiHandler Application::imgui_manager;
    graphicpipe::GraphicsPipe* pipe;
    Input::InputSystem Input;
    assetmanager::AssetManager* AstManager;
    logging::Logger logs;


    // Audio Demo timer
    float audioTimer = 3.0f;
    bool audio2_bool = true;

    float LastTime = static_cast<float>(glfwGetTime());
    

    int Application::Init() {
        

        /*--------------------------------------------------------------
        INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS("Debugging/LogFile.txt");
        LOGGING_INFO("Application Start");
        LOGGING_INFO("Load Log Successful");
        logs.m_Setup_Abort_Handler();
        std::signal(SIGABRT, logging::Logger::m_Abort_Handler);

        /*--------------------------------------------------------------
          INITIALIZE WINDOW WIDTH & HEIGHT
       --------------------------------------------------------------*/
        Serialization::Serialize::LoadConfig();
        LOGGING_INFO("Load Config Successful");

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        AstManager = assetmanager::AssetManager::m_funcGetInstance();
        AstManager->m_funcLoadAssets();
        LOGGING_INFO("Load Asset Successful");


        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        pipe->m_funcInit();
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
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs->m_Load();
        ecs->m_Init();
        LOGGING_INFO("Load ECS Successful");

        LOGGING_INFO("Application Init Successful");
    
        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
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
                ecs->m_Update(Helper::Helpers::GetInstance()->DeltaTime);

                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                    --------------------------------------------------------------*/
                pipe->m_funcUpdate();

                /*--------------------------------------------------------------
                    DRAWING/RENDERING Window
                    --------------------------------------------------------------*/
                lvWindow.Draw();

                /*--------------------------------------------------------------
                    DRAWING/RENDERING Objects
                    --------------------------------------------------------------*/
                pipe->m_funcDrawWindow();


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
        ecs::ECS::m_GetInstance()->m_Unload();
        imgui_manager.Shutdown();
        lvWindow.CleanUp();
        glfwTerminate();
        LOGGING_INFO("Application Closed");

        return 0;
	}

}