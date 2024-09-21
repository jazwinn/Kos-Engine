
#include "Application.h"

#include "../Graphics/GraphicsPipe.h"
#include "../Assets/AssetManager.h"
#include "../Inputs/Input.h"
#include "../ECS/ECS.h"
#include "Window.h"


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"

namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    AppWindow lvWindow;
    ImGuiHandler imgui_manager;
    GraphicsPipe* pipe;
    AssetManager* AstManager;
    Input::InputSystem Input;

    float LastTime = static_cast<float>(glfwGetTime());

    int Application::Init() {
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
            /*--------------------------------------------------------------
             IMGUI FRAME SETUP
             --------------------------------------------------------------*/
            imgui_manager.NewFrame();

            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // <----- Is this needed?
            imgui_manager.DrawHierachyWindow(clear_color);


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
            lvWindow.Draw(clear_color);

            /*--------------------------------------------------------------
             DRAWING/RENDERING Objects
             --------------------------------------------------------------*/
             //TODO remove paremeter, less hard code
            pipe->funcDrawWindow();

            /*--------------------------------------------------------------
             Draw IMGUI FRAME
             --------------------------------------------------------------*/
            //Size of the render window is based on the parameters of this function
            imgui_manager.DrawRenderScreenWindow(lvWin->funcGetWinWidth()/2, lvWin->funcGetWinHeight()/2);
            imgui_manager.Render();


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

        return 0;
	}

}