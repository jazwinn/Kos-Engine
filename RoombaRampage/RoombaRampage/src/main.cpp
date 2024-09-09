#include "Graphics.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_handler.h"
#include <iostream>

//Initialize shader strings here for now
const std::string testVertexShader =
{
 #include "testVertexShader.vert"
};

const std::string testFragmentShader =
{
  #include "testFragmentShader.frag"
};

const std::string genericVertexShader =
{
 #include "genericVertexShader.vert"
};

const std::string genericFragmentShader =
{
  #include "genericFragmentShader.frag"
};

    int main(void)
    {

        /*--------------------------------------------------------------
           INITIALIZE OPENGL WINDOW
        --------------------------------------------------------------*/
        GLFWwindow* window;
        
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(1000, 1000, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            return -1;
        }
        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Only initialize GLEW after defining OpenGL context*/
        if (glewInit() != GLEW_OK)
        {
            std::cout << "Failed to initialize GLEW" << std::endl;
            return -1;
        }
        
        /*--------------------------------------------------------------
            INITIALIZE OBJECTS
         --------------------------------------------------------------*/
        GraphicsObject testRect(GraphicsObject::RECTANGLE);
        testRect.setup_vao();
        testRect.set_rotate(45.f);
        testRect.set_translate(0.5f,0.75f);
        testRect.set_scale(2.f,2.f);
        GraphicsObject::set_draw_mode(GL_FILL);
        testRect.setup_shader(genericVertexShader, genericFragmentShader);

        /*--------------------------------------------------------------
           INITIALIZE IMGUI
        --------------------------------------------------------------*/

        ImGuiHandler imgui_manager;
        const char* glsl_version = "#version 130";
        imgui_manager.Initialize(window, glsl_version);

        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        //io.Fonts->AddFontDefault();
        //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
        //IM_ASSERT(font != nullptr);
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /*--------------------------------------------------------------
           GAME LOOP
        --------------------------------------------------------------*/

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            /*--------------------------------------------------------------
             UPDATE OBJECTS
             --------------------------------------------------------------*/

            testRect.add_translate(-0.001f, -0.001f);
            testRect.update();


            /*--------------------------------------------------------------
             IMGUI FRAME SETUP
             --------------------------------------------------------------*/
            imgui_manager.NewFrame();
            imgui_manager.DrawCustomWindow(show_demo_window, show_another_window, clear_color);

            /*--------------------------------------------------------------
             DRAWING/RENDERING
             --------------------------------------------------------------*/
            glClear(GL_COLOR_BUFFER_BIT);
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);

            testRect.draw();
            imgui_manager.Render();

            glfwSwapBuffers(window);
        }


        /*-----------------------------------------------------------
        CLEANUP
        --------------------------------------------------------------*/
        testRect.delete_shader();
        imgui_manager.Shutdown();
        glfwDestroyWindow(window);
        glfwTerminate();
        return 0;
    }