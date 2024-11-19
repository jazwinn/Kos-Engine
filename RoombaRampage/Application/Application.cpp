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




#include "../Config/pch.h"
#include "Application.h"

#include "../Graphics/GraphicsPipe.h"
<<<<<<< HEAD
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/SceneManager.h"
#include "../Events/EventHandler.h"
#include "../Inputs/Input.h"
=======
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

>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
<<<<<<< HEAD
   #define IMGUIENABLED
=======
    AppWindow Application::lvWindow;
    gui::ImGuiHandler Application::imgui_manager; 
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
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
            LOAD ENTITIES INTO ECS & IMGUI
        --------------------------------------------------------------*/
<<<<<<< HEAD
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        scenemanager->m_LoadScene("Assets/Scene/Default.json"); // replace with opening up window dialog
        
        /*--------------------------------------------------------------
            INITIALIZE EDITOR // LAST INIT
         --------------------------------------------------------------*/
#ifdef IMGUIENABLED
        const char* glsl_version = "#version 130";
        Editor.m_Initialize(lvWindow.m_window, glsl_version);
        LOGGING_INFO("Load ImGui Successful");

#endif 

#ifdef IMGUIENABLED
        std::cout << "IMGUIENABLED is defined" << std::endl;
#else
        std::cout << "IMGUIENABLED is NOT defined" << std::endl;
        pipe->m_gameMode = true;
#endif      

=======
        AstManager->m_loadEntities("../RoombaRampage/Json/components.json");
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128

        LOGGING_INFO("Application Init Successful");
    
        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        //float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();
<<<<<<< HEAD
        const double fixedDeltaTime = 1.0 / 60.0;
        help->m_fixedDeltaTime = static_cast<float>(fixedDeltaTime);
        ecs->m_DeltaTime = static_cast<float>(fixedDeltaTime);
        double accumulatedTime = 0.0;

=======
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
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
<<<<<<< HEAD
                --------------------------------------------------------------*/
                ecs->m_Update(help->m_fixedDeltaTime); 
                //ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);
=======
                    --------------------------------------------------------------*/
                ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128

                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                    --------------------------------------------------------------*/
                pipe->m_funcUpdate();



                /*--------------------------------------------------------------
                    DRAWING/RENDERING Window
                    --------------------------------------------------------------*/
                lvWindow.Draw();
#ifdef IMGUIENABLED

                /*--------------------------------------------------------------
                    DRAWING/RENDERING Objects
                    --------------------------------------------------------------*/
                pipe->m_funcDrawWindow();


                /*--------------------------------------------------------------
                    Draw IMGUI FRAME
<<<<<<< HEAD
                --------------------------------------------------------------*/
                Editor.m_Render();
#endif
                /*--------------------------------------------------------------
                   Render Game Scene
                --------------------------------------------------------------*/
                pipe->m_funcRenderGameScene();
           

              

                //double currentFrameTime = glfwGetTime();
                //help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
=======
                    --------------------------------------------------------------*/
                imgui_manager.m_Render();

          
                /*--------------------------------------------------------------
                    Calculate time
                    --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128

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



	int Application::Cleanup() {
        ecs::ECS::m_GetInstance()->m_Unload();
#ifdef IMGUIENABLED
        Editor.m_Shutdown();
#endif
        lvWindow.CleanUp();
        glfwTerminate();
        LOGGING_INFO("Application Closed");

        return 0;
	}

}