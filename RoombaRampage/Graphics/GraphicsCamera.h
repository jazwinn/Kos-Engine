#ifndef GRAPHCAM_H
#define GRAPHCAM_H

#include <glm.hpp>
#include <vector>
#include <memory>

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
   

        static void calculateAspectRatio();
        static void multiplyActiveCameraMatrix();
        static void calculateCurrCamera();
        static void calculateCurrView();
        static void setCurrCamera(unsigned int index);
        static void multiplyOrthoMatrix();
        static void multiplyViewMatrix();

        static GraphicsCamera* m_funcGetInstance();

        static glm::mat3 m_currCameraMatrix;                ///< Current camera matrix
        static glm::mat3 m_currViewMatrix;
        static glm::mat3 m_currOrthoMatrix;

        static glm::mat3 m_currCamera;
        static std::vector<glm::mat3> m_cameras;

    private:
        static std::unique_ptr<GraphicsCamera> m_instancePtr;
	};
}

#endif GRAPHCAM_H