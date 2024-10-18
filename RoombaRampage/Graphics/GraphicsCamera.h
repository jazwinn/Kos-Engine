#ifndef GRAPHCAM_H
#define GRAPHCAM_H

#include <glm.hpp>
#include <vector>

namespace graphicpipe

{
	class GraphicsCamera
	{
    public:
        static int m_windowWidth;                        ///< The current window width.
        static int m_windowHeight;                       ///< The current window height.
        static float m_aspectRatio;                      ///< Aspect ratio of the window.

        /**
         * @struct Camera
         * @brief Represents the camera's position and orientation.
         */
        struct OrthoCamera                
        {
            glm::vec2 m_coordinates; 
            glm::vec2 m_zoom;         
            float m_angle;            
        };

        static void calculateAspectRatio();
        static void multiplyActiveCameraMatrix();
        static void calculateCurrCamera();
        static void setCurrCamera(unsigned int index);

        static GraphicsCamera* m_funcGetInstance();

        static glm::mat3 m_currCameraMatrix;                ///< Current camera matrix

        static OrthoCamera m_currCamera;
        static std::vector<OrthoCamera> m_cameras;

    private:
        static std::unique_ptr<GraphicsCamera> m_instancePtr;
	};
}

#endif GRAPHCAM_H