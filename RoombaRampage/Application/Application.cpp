/******************************************************************/
/*!
\file      Application.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the main update loop of the engine.
           It initializes the various major systems and call upon
           them in the update loop. When the window is closed,
           the applciations cleanup function is called to 
           call the major systems to clean their program.
           


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
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
#include "../C# Mono/mono_handler.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    AppWindow Application::lvWindow;
    gui::ImGuiHandler Application::imgui_manager; 
    graphicpipe::GraphicsPipe* pipe;
    Input::InputSystem Input;
    assetmanager::AssetManager* AstManager;
    logging::Logger logs;
    mono::MonoScriptHandler monoManager;  

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
        Serialization::Serialize::m_LoadConfig();
        LOGGING_INFO("Load Config Successful");

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs->m_Load();
        ecs->m_Init();
        LOGGING_INFO("Load ECS Successful");
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
        Input.SetCallBack(lvWindow.m_window);
        LOGGING_INFO("Set Input Call Back Successful");
        

       /*--------------------------------------------------------------
           INITIALIZE IMGUI
        --------------------------------------------------------------*/
        const char* glsl_version = "#version 130";
        imgui_manager.m_Initialize(lvWindow.m_window, glsl_version);
        LOGGING_INFO("Load ImGui Successful");

        /*--------------------------------------------------------------
            LOAD ENTITIES INTO ECS & IMGUI
        --------------------------------------------------------------*/
        AstManager->m_loadEntities("../RoombaRampage/Json/components.json");

        LOGGING_INFO("Application Init Successful");

        /*--------------------------------------------------------------
           INITIALIZE MONO AND ASSEMBLY LOADING
       --------------------------------------------------------------*/
       // Mono initialization and assembly loading
        if (!monoManager.m_InitMono("C# Mono/ExampleScript.dll")) {
            return -1;
        }

        // Load the HelloWorld method
        if (!monoManager.m_LoadMethod("ExampleScript", "HelloWorld", 0)) {
            return -1; 
        }

        // Load the HelloWorld method
        if (!monoManager.m_LoadMethod("ExampleScript", "PrintMessage", 2)) {
            return -1;
        }

        LOGGING_INFO("Mono initialization and method loading successful.");

        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();

        // Invoke the HelloWorld method
        monoManager.m_InvokeMethod("ExampleScript", "HelloWorld", nullptr, 0);

        // Invoke the PrintMessage
        int number = 42;
        MonoString* message = mono_string_new(monoManager.m_GetMonoDomain(), "Calling Method 2!");
        void* args[2] = { &number, message };
        monoManager.m_InvokeMethod("ExampleScript", "PrintMessage", args, 2);

        /*--------------------------------------------------------------
            GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.m_window))
        {
            try {
                /* Poll for and process events */
                glfwPollEvents();

                /*--------------------------------------------------------------
                    UPDATE ECS
                    --------------------------------------------------------------*/
                ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);

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
                imgui_manager.m_Render();

          
                /*--------------------------------------------------------------
                    Calculate time
                    --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);

                while (help->m_deltaTime < FPSCapTime) {
                    lastFrameTime = currentFrameTime;
                    currentFrameTime = glfwGetTime();
                    help->m_deltaTime += static_cast<float>(currentFrameTime - lastFrameTime);
                }
                lastFrameTime = glfwGetTime();
                help->m_fps = 1.f / help->m_deltaTime;

                glfwSwapBuffers(lvWindow.m_window);
            }
            catch (const std::exception& e) {
                LOGGING_ERROR("Exception in game loop: {}", e.what());
            }
        }
        return 0;
    }



	int Application::m_Cleanup() {
        ecs::ECS::m_GetInstance()->m_Unload();
        imgui_manager.m_Shutdown();
        lvWindow.CleanUp();
        glfwTerminate();
        LOGGING_INFO("Application Closed");

        return 0;
	}

}