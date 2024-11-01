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
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"
#include "../Events/EventHandler.h"
#include "../Inputs/Input.h"


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    
    graphicpipe::GraphicsPipe* pipe;
    assetmanager::AssetManager* AstManager;

    

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
           INITIALIZE MONO AND ASSEMBLY LOADING
       --------------------------------------------------------------*/
        /****************************************************************************************/
        //SAMPLE TO REMOVE
        //Compiles .cs
        assetmanager::AssetManager::m_funcGetInstance()->m_scriptManager.m_CompileCSharpFile("C:/Users/ngjaz/OneDrive/Documents/roombarampage/GreyGooseWorkspace/RRR/RoombaRampage/Assets/Asset/Scripts/Scripts CS/MovementScript.cs");
        /****************************************************************************************/

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        AstManager = assetmanager::AssetManager::m_funcGetInstance();
        AstManager->m_funcLoadAssets("Assets");
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
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        scenemanager->m_LoadScene("../RoombaRampage/Assets/Scene/Default.json"); // replace with opening up window dialog
        
        ////scenemanager->m_LoadScene("Scene1");

        

       


        LOGGING_INFO("Mono initialization and method loading successful.");


        LOGGING_INFO("Application Init Successful");
        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        //float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();
        const double fixedDeltaTime = 1.0 / 60.0;
        help->m_fixedDeltaTime = static_cast<float>(fixedDeltaTime);
        ecs->m_DeltaTime = static_cast<float>(fixedDeltaTime);
        double accumulatedTime = 0.0;

        /*--------------------------------------------------------------
            GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.m_window))
        {
            try {
                /* Poll for and process events */
                glfwPollEvents();

                ///*--------------------------------------------------------------
                //    Calculate time
                // --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
                lastFrameTime = currentFrameTime;

                accumulatedTime += help->m_deltaTime;
                Helper::Helpers::GetInstance()->currentNumberOfSteps = 0;
                while (accumulatedTime >= fixedDeltaTime) {
                    accumulatedTime -= fixedDeltaTime;
                    ++help->currentNumberOfSteps;
                }

                /*--------------------------------------------------------------
                    UPDATE INPUT
                --------------------------------------------------------------*/
                Input.m_inputUpdate();

                /*--------------------------------------------------------------
                    UPDATE ECS
                --------------------------------------------------------------*/
                ecs->m_Update(help->m_fixedDeltaTime); 
                //ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);

                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                --------------------------------------------------------------*/
                pipe->m_funcUpdate();

                /*--------------------------------------------------------------
                    DRAWING/RENDERING Window
                --------------------------------------------------------------*/
                lvWindow.Draw();

                /*--------------------------------------------------------------
                    Draw IMGUI FRAME
                --------------------------------------------------------------*/
                imgui_manager.m_Render();

                /*--------------------------------------------------------------
                   Render Game Scene
                --------------------------------------------------------------*/
                pipe->m_funcRenderGameScene();
               

                //double currentFrameTime = glfwGetTime();
                //help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);

                //while (help->m_deltaTime < FPSCapTime) {
                //    lastFrameTime = currentFrameTime;
                //    currentFrameTime = glfwGetTime();
                //    help->m_deltaTime += static_cast<float>(currentFrameTime - lastFrameTime);
                //}
                //lastFrameTime = glfwGetTime();
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