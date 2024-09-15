#include "Application.h"


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    Application* Application::lvInstance = nullptr;
    //AppWindow lvWindow;
    ImGuiHandler imgui_manager;
    GraphicsPipe* pipe;


    Application::Application() {
        /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        std::string lvTitle{"Roomba Rampage"};
        int lvWidth = 1280, lvHeight = 720;
        winProperties lvProps{};

        lvProps.lvWinTitle = lvTitle;
        lvProps.lvWinWidth = lvWidth;
        lvProps.lvWinHeight = lvHeight;

        lvInstance = this;
        lvWin = std::unique_ptr<AppWindow>(AppWindow::funcCreateWindow(lvProps));
        lvWin->funcSetEventCallback(BIND_EVENT_FN(funcEvent));

        monitor = glfwGetPrimaryMonitor();
        mode = glfwGetVideoMode(monitor);
        lvWinSize.x = static_cast<float>(Application::funcGetApp().funcGetWin().funcGetWinWidth());
        lvWinSize.y = static_cast<float>(Application::funcGetApp().funcGetWin().funcGetWinHeight());

        lvWinWidth = mode->width;
        lvWinHeight = mode->height;
        glfwGetWindowPos(static_cast<GLFWwindow*>(Application::funcGetApp().funcGetWin().funcGetNatWin()), &lvWinPosX, &lvWinPosY);

        
	}

    Application::~Application() {
        ECS* ecs = ECS::GetInstance();
        ecs->Unload();//replace with free function when available
    }
   
    void Application::funcEvent(classEvent& givenEvent) {
        classEventDispatch lvDispatcher(givenEvent);
        lvDispatcher.funcDispatch<classWindowClosedEvent>(BIND_EVENT_FN(funcOnWinClose));
        //lvDispatcher.funcDispatch<classWindowResizeEvent>(BIND_EVENT_FN(funcOnWinResize));
        lvDispatcher.funcDispatch<classMouseMoveEvent>(BIND_EVENT_FN(funcOnMouseMove));
    }

    Application& Application::funcGetApp() {
        return *lvInstance;
    }

    bool Application::funcOnWinClose(classWindowClosedEvent& givenEvent) {
        lvIsRunning = false;
        return true;
    }
    //bool Application::funcOnWinResize(classWindowResizeEvent& givenEvent) {
    //   
    //    return true;
    //}
    bool Application::funcOnMouseMove(classMouseMoveEvent& givenEvent) {
        lvMousePos.x = givenEvent.funcGetX();
        lvMousePos.y = givenEvent.funcGetY();
        return true;
    }


    int Application::Init() {
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

            /*--------------------------------------------------------------
             IMGUI FRAME SETUP
             --------------------------------------------------------------*/
            imgui_manager.NewFrame();

            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // <----- Is this needed?


            imgui_manager.DrawHierachyWindow(show_demo_window, show_another_window, clear_color);


            /*--------------------------------------------------------------
             UPDATE ECS
             --------------------------------------------------------------*/
            ecs->Update();

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

            glfwSwapBuffers(static_cast<GLFWwindow*>(Application::funcGetApp().funcGetWin().funcGetNatWin()));
            if (classInput::funcIsKeyPress(GLFW_KEY_Q)) {
                glfwTerminate();
            }
        }

        return 0;
	}


	int Application::Cleanup() {

        ECS::GetInstance()->Unload();
        imgui_manager.Shutdown();
        //funcGetApp().funcGetWin().CleanUp();
        glfwTerminate();

        return 0;
	}

}