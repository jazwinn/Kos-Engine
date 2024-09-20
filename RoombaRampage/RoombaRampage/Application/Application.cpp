
#include "Application.h"

#include "../Graphics/GraphicsPipe.h"
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
    std::shared_ptr<Application> Application::lvInstance;
    ImGuiHandler imgui_manager;
    GraphicsPipe* pipe;
    std::shared_ptr<AppWindow> Application::lvWin;
    Input::classInput gvInput;
 


   
    void Application::funcEvent(classEvent& givenEvent) {
        classEventDispatch lvDispatcher(givenEvent);
        lvDispatcher.funcDispatch<classMouseMoveEvent>(BIND_EVENT_FN(funcOnMouseMove));
    }


    bool Application::funcOnMouseMove(classMouseMoveEvent& givenEvent) {
        lvMousePos.x = givenEvent.funcGetX();
        lvMousePos.y = givenEvent.funcGetY();
        return true;
    }


    float LastTime = glfwGetTime();;


    int Application::Init() {
       
        
        std::string lvTitle{ "Roomba Rampage" };
        int lvWidth = 1280, lvHeight = 720;
        winProperties lvProps{};

        lvProps.lvWinTitle = lvTitle;
        lvProps.lvWinWidth = lvWidth;
        lvProps.lvWinHeight = lvHeight;
        lvInstance = std::make_shared<Application>(); 
        lvWin = std::make_shared<AppWindow>(lvProps);
        

        lvWin->funcSetEventCallback(std::bind(&Application::funcEvent, lvInstance, std::placeholders::_1));

        GLFWmonitor* lvMon = glfwGetPrimaryMonitor();
        const GLFWvidmode* lvMode = glfwGetVideoMode(lvMon);
        

        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/

        pipe = GraphicsPipe::funcGetInstance();
        pipe->funcInit();

        /*--------------------------------------------------------------
           INITIALIZE IMGUI
        --------------------------------------------------------------*/
        const char* glsl_version = "#version 130";
        imgui_manager.Initialize(static_cast<GLFWwindow*>(funcGetApp().funcGetWin().funcGetNatWin()), glsl_version);

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
        /*--------------------------------------------------------------
         GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(funcGetApp().funcGetWin().funcGetNatWin())))
        {
            /* Poll for and process events */
            glfwPollEvents();

            //calculate DeltaTime
            float CurrentTime = glfwGetTime();
            float DeltaTime =  CurrentTime - LastTime;
            LastTime = CurrentTime;
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
            funcGetApp().funcGetWin().Draw(clear_color);

            /*--------------------------------------------------------------
             DRAWING/RENDERING Objects
             --------------------------------------------------------------*/
             //TODO remove paremeter, less hard code
            pipe->funcDraw(pipe->squareMesh);


            /*--------------------------------------------------------------
             Draw IMGUI FRAME
             --------------------------------------------------------------*/
            imgui_manager.Render();


            glfwSwapBuffers(static_cast<GLFWwindow*>(funcGetApp().funcGetWin().funcGetNatWin()));
        }

        return 0;
	}


	int Application::Cleanup() {
        Ecs::ECS::GetInstance()->Unload();
        imgui_manager.Shutdown();
        funcGetApp().funcGetWin().funcWinShutdown();
        glfwTerminate();

        return 0;
	}

}