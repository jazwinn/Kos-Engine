/******************************************************************/
/*!
\file      GraphicsPipe.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     The GraphicsPipe class provides a high-level interface for setting up and managing OpenGL rendering,
           including shaders, framebuffers, VAOs, and drawing operations. It supports various graphical
           data types, including meshes, textures, debug drawing, and text rendering. It also includes
           functionality for animation and frame management.

\details    This class is designed to handle multiple rendering operations, such as instanced drawing,
            managing different textures, and setting up framebuffers for offscreen rendering. It also includes
            functions for managing shader programs and rendering text using FreeType.
 
             Dependencies:
            -GLEW for OpenGL extension loading.
            -GLFW for window and context management.
            -GLM for matrix operations and transformations.
            -AssetManager for loading assets such as fonts and images.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

/**
*  @autho
 * @file GraphicsPipe.h
 * @brief Defines the GraphicsPipe class and related data structures for handling graphics rendering using OpenGL.
 *
 * The GraphicsPipe class provides a high-level interface for setting up and managing OpenGL rendering,
 * including shaders, framebuffers, VAOs, and drawing operations. It supports various graphical
 * data types, including meshes, textures, debug drawing, and text rendering. It also includes
 * functionality for animation and frame management.
 *
 * @details This class is designed to handle multiple rendering operations, such as instanced drawing,
 * managing different textures, and setting up framebuffers for offscreen rendering. It also includes
 * functions for managing shader programs and rendering text using FreeType.
 *
 * Dependencies:
 * - GLEW for OpenGL extension loading.
 * - GLFW for window and context management.
 * - GLM for matrix operations and transformations.
 * - AssetManager for loading assets such as fonts and images.
 */

#ifndef GRAPHPIPE_H
#define GRAPHPIPE_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <string>
#include <vector>
#include <memory>
#include "../Assets/AssetManager.h"

namespace graphicpipe {

    /**
     * @struct GraphicsData
     * @brief Stores the data required for rendering a graphical object.
     */
    struct GraphicsData
    {
        //float m_rotate{};                ///< Rotation of the object in degrees.
        //glm::vec2 m_scale{};             ///< Scale factors for the object (x, y).
        //glm::vec2 m_worldCoordinates{};  ///< World coordinates of the object.
        glm::mat3 m_transformation{};
        unsigned int m_textureID{};      ///< ID of the texture used for rendering.
        int m_frameNumber{};               ///< Frame Number for handling animations.
        int m_layer{};                   ///< Layer for drawing order.
    };

    /**
     * @struct DebugDrawData
     * @brief Stores the data required for rendering debug information.
     */
    struct DebugDrawData
    {
       /* float m_rotate{};                ///< Rotation of the debug shape in degrees.
        glm::vec2 m_scale{};             ///< Scale factors for the debug shape (x, y).
        glm::vec3 m_worldCoordinates{};  ///< World coordinates of the debug shape.*/
        glm::mat3 m_transformation{};
        bool m_isCollided{};             ///< Collision flag for detecting collisions.
        int m_shapeType{};               ///< Type of debug shape (e.g., square, circle).
    };

    /**
     * @struct TextData
     * @brief Stores data required for rendering text.
     */
    struct TextData
    {
        std::string m_text;              ///< The text string to be rendered.
        float m_x;                       ///< X position of the text on screen.
        float m_y;                       ///< Y position of the text on screen.
        float m_scale;                   ///< Scale of the text.
        glm::vec3 m_color;               ///< Color of the text (RGB).
    };

    /**
     * @class GraphicsPipe
     * @brief Manages the rendering pipeline, shaders, and drawing functions.
     *
     * The GraphicsPipe class handles the setup and rendering of graphical objects,
     * including mesh data, framebuffers, shaders, and textures. It also provides
     * functionality for instanced rendering, debug drawing, and text rendering.
     */
    class GraphicsPipe
    {
    private:

        int m_unitWidth{ 512 };         ///< The default width for the graphics unit.
        int m_unitHeight{ 512 };        ///< The default height for the graphics unit.

        /**
         * @struct Mesh
         * @brief Stores mesh data for rendering.
         */
        struct Mesh
        {
            int m_shapeType{};           ///< Shape type (square, circle, etc.).
            unsigned int m_vaoId{};      ///< VAO ID for this mesh.
            unsigned int m_primitiveType{}; ///< OpenGL primitive type (e.g., GL_TRIANGLES).
            unsigned short m_indexElementCount{}; ///< Number of elements to draw.
        };

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering text.
         */
        void m_funcSetupTextVao();

        /**
         * @brief Sets up the Vertex Array Object (VAO) for a specific mesh.
         *
         * @param shape Reference to the Mesh structure to set up.
         */
        void m_funcSetupVao(Mesh& shape);

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering to a framebuffer.
         */
        void m_funcSetupFboVao();

        /**
         * @brief Sets up the Vertex Array Object (VAO) for rendering square outlines.
         */
        void m_funcSetupSquareLinesVao();

        /**
         * @brief Sets up the array buffer for storing model matrix data.
         */
        void m_funcSetupArrayBuffer();

       

        /**
         * @brief Compiles and links a shader program from vertex and fragment shader sources.
         *
         * @param vertexShader The source code of the vertex shader.
         * @param fragmentShader The source code of the fragment shader.
         * @return The compiled and linked shader program ID.
         */
        unsigned int m_funcSetupShader(const std::string& vertexShader, const std::string& fragmentShader);

        /**
         * @brief Deletes the currently active shader program.
         */
        void m_funcDeleteShader();

        static std::unique_ptr<GraphicsPipe> m_instancePtr; ///< Singleton instance of the GraphicsPipe class.

        // Shader Programs
        unsigned int m_genericShaderProgram{};      ///< Shader program for general rendering.
        unsigned int m_frameBufferShaderProgram{};  ///< Shader program for framebuffer rendering.
        unsigned int m_debugShaderProgram{};        ///< Shader program for debug rendering.
        unsigned int m_textShaderProgram{};         ///< Shader program for text rendering.

        // Buffers
        unsigned int m_modelMatrixArrayBuffer{};    ///< Array buffer for model matrices.
        unsigned int m_debugMatrixArrayBuffer{};    ///< Array buffer for debug matrices.
        unsigned int m_textureOrderBuffer{};        ///< Buffer for texture ordering.
        unsigned int m_debugCollisionCheckBuffer{}; ///< Buffer for collision detection in debug drawing.
        unsigned int m_frameBufferObject{};         ///< Framebuffer object for offscreen rendering.
        unsigned int m_depthBufferObject{};         ///< Depth Buffer object for storing frame buffer data.
        unsigned int m_textBuffer{};                ///< Buffer for text rendering.
        unsigned int m_stripCountBuffer{};          ///< Buffer for sprite strip counts (animation).
        unsigned int m_frameNumberBuffer{};         ///< Buffer for managing animation frame numbers.

        glm::mat3 m_testMatrix{};                   ///< Test matrix for rendering.

        /**
         * @brief Compiles an individual shader from source.
         *
         * @param type The type of the shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER).
         * @param shader The shader source code.
         * @return The compiled shader ID.
         */
        static GLuint m_funcCompileShader(GLuint type, const std::string& shader);

    public:

       

        /**
         * @enum ShapeType
         * @brief Enumerates different shape types for rendering.
         */
        enum ShapeType
        {
            SQUARE,       ///< Square shape type.
            CIRCLE,       ///< Circle shape type.
            SQUARE_LINES, ///< Square outline shape type.
            CIRCLE_LINES  ///< Circle outline shape type.
        };

        /**
         * @brief Destructor for the GraphicsPipe class.
         *
         * Cleans up resources such as framebuffers and shader programs.
         */
        ~GraphicsPipe();

        /**
         * @brief Returns the singleton instance of the GraphicsPipe class.
         *
         * Creates a new instance if it doesn't already exist.
         *
         * @return Pointer to the singleton GraphicsPipe instance.
         */
        static GraphicsPipe* m_funcGetInstance();

        /**
         * @brief Initializes the graphics pipeline, setting up VAOs, shaders, and framebuffers.
         *
         * This function is responsible for enabling blending, setting line width, and initializing
         * various buffers and data structures needed for rendering. It sets up the VAOs for the
         * square, square lines, and text meshes, as well as framebuffers for offscreen rendering.
         * Additionally, it compiles and links shader programs for generic rendering, framebuffer
         * rendering, debug drawing, and text rendering. It reserves memory for storing data for
         * entities, textures, and debugging, ensuring the system can handle the maximum number of entities.
         *
         * Shader programs are loaded for different drawing purposes, and the window size and aspect ratio
         * are obtained from helper functions. Lastly, the function enables the scissor test, used for
         * limiting drawing to a specified area of the window.
         *
         * @note Some code for monitor setup and debug callbacks is commented out but can be uncommented if needed.
         */
        void m_funcInit();

        /**
         * @brief Updates the graphics pipeline.
         *
         * This function should be called every frame to update any dynamic aspects of the rendering pipeline,
         * such as animations, transformations, or other time-dependent changes.
         */
        void m_funcUpdate();

        /**
         * @brief Draws the entire window.
         *
         * Handles rendering all graphical elements to the window, including models, debug information, and text.
         */
        void m_funcDrawWindow();

        /**
         * @brief Draws all graphical elements.
         *
         * Renders all objects stored in the graphics data structures to the screen.
         */
        void m_funcDraw();

        /**
         * @brief Draws debug elements.
         *
         * Renders debug-specific elements, such as collision boxes or other diagnostic visuals.
         */
        void m_funcDrawDebug();

        /**
         * @brief Sets the drawing mode for rendering.
         *
         * @param mode The OpenGL drawing mode (e.g., GL_TRIANGLES, GL_LINES).
         */
        static void m_funcSetDrawMode(GLenum mode);

        /**
         * @brief Draws text using the text shader.
         *
         * Renders all text elements stored in the text data structures to the screen.
         */
        void m_funcDrawText();

        /**
        * @brief Sets up the framebuffer for offscreen rendering.
        */
        void m_funcSetupFrameBuffer();

        void m_funcCalculateModelToWorldMatrix();

        void m_funcRenderGameScene();

        //Boolean Values
        bool m_gameMode{ false };

        // Meshes
        Mesh m_squareMesh;              ///< Mesh for square rendering.
        Mesh m_screenMesh;              ///< Mesh for screen rendering.
        Mesh m_squareLinesMesh;         ///< Mesh for rendering square outlines.
        Mesh m_textMesh;                ///< Mesh for text rendering.

        // Matrix Containers
        std::vector<glm::mat3> m_modelToNDCMatrix{}; ///< Model-to-NDC transformation matrices.
        std::vector<glm::mat3> m_debugToNDCMatrix{}; ///< Debug model-to-NDC matrices.

        // Data for rendering
        std::vector<GraphicsData> m_modelData{}; ///< Graphics data for rendering.
        std::vector<DebugDrawData> m_debugBoxData{}; ///< Data for rendering debug boxes.
        std::vector<TextData> m_textData{}; ///< Data for rendering text.
        std::vector<float> m_debugBoxCollisionChecks{}; ///< Collision check data for debug rendering.
        std::vector<int> m_textureOrder{}; ///< Order of texture bindings.

        // Texture IDs
        std::vector<unsigned int> m_textureIDs{}; ///< Array of texture IDs.
        std::vector<int> m_stripCounts{}; ///< Sprite strip counts for animation.
        std::vector<int> m_frameNumbers{}; ///< Frame numbers for sprite animations.
        std::vector<image::Image> m_imageData{}; ///< Image data for rendering.

        unsigned int m_screenTexture{}; ///< Texture for rendering the screen.
        unsigned int m_proxyBackgroundTexture{}; ///< Background texture for proxy rendering.

        //Shaders
        const std::string debugVertexShader =
        {
        #include "../Graphics/debugVertexShader.vert"
        };

        const std::string debugFragmentShader =
        {
        #include  "../Graphics/debugFragmentShader.frag"

        };

        const std::string frameBufferVertexShader =
        {
         #include "../Graphics/frameBufferVertexShader.vert"
        };

        const std::string frameBufferFragmentShader =
        {
          #include "../Graphics/frameBufferFragmentShader.frag"
        };


        const std::string genericVertexShader =
        {
         #include "../Graphics/genericVertexShader.vert"
        };

        const std::string genericFragmentShader =
        {
          #include "../Graphics/genericFragmentShader.frag"
        };

        const std::string textVertexShader =
        {
         #include "../Graphics/textVertexShader.vert"
        };

        const std::string textFragmentShader =
        {
          #include "../Graphics/textFragmentShader.frag"
        };
    };

} // namespace graphicpipe

#endif // GRAPHPIPE_H
