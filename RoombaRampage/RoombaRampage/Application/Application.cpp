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
    Application* Application::lvInstance = nullptr;
    //AppWindow lvWindow;
    ImGuiHandler imgui_manager;
    GraphicsPipe* pipe;
    auto app = new Application();

    std::unique_ptr<AppWindow> Application::lvWin;
 


   
    void Application::funcEvent(classEvent& givenEvent) {
        classEventDispatch lvDispatcher(givenEvent);
        lvDispatcher.funcDispatch<classMouseMoveEvent>(BIND_EVENT_FN(funcOnMouseMove));
    }

    Application& Application::funcGetApp() {
        return *lvInstance;
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
        lvInstance = new Application;
        lvWin = std::unique_ptr<AppWindow>(AppWindow::funcCreateWindow(lvProps));
        

        lvWin->funcSetEventCallback(std::bind(&Application::funcEvent, app, std::placeholders::_1));

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
        imgui_manager.Initialize(static_cast<GLFWwindow*>(Application::funcGetApp().funcGetWin().funcGetNatWin()), glsl_version);


        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        //fetch ecs
        ECS* ecs = ECS::GetInstance();

        ecs->Load();
        ecs->Init();

        return 0;
    }



    int Application::Run() {

        ECS* ecs = ECS::GetInstance();
        /*--------------------------------------------------------------
         GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(Application::funcGetApp().funcGetWin().funcGetNatWin())))
        {
            /* Poll for and process events */
            //glfwPollEvents();

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


            funcGetApp().funcGetWin().funcUpdate();

            glfwSwapBuffers(static_cast<GLFWwindow*>(funcGetApp().funcGetWin().funcGetNatWin()));

            if (Input::classInput::funcIsKeyPress(GLFW_KEY_Q)) {
                std::cout << "CHECKING!" << std::endl;
                funcGetApp().isRunning = false;
                //Cleanup();
            }


            glfwSwapBuffers(static_cast<GLFWwindow*>(funcGetApp().funcGetWin().funcGetNatWin()));
            if (funcGetApp().isRunning == false) {
                Application::Cleanup();
            }
        }
        
        return 0;
	}


	int Application::Cleanup() {

        ECS::GetInstance()->Unload();
        imgui_manager.Shutdown();
        //funcGetApp().funcGetWin().funcGetNatWin();
        funcGetApp().funcGetWin().funcWinShutdown();
        //funcGetApp().funcGetWin().CleanUp();
        //glfwTerminate();

        return 0;
	}

}