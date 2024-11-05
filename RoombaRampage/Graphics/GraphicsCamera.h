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
   

        static void calculateAspectRatio();
        static void calculateCurrView();
        static void setCurrCamera(unsigned int index);
        static void multiplyOrthoMatrix();
        static void multiplyViewMatrix();

        static GraphicsCamera* m_funcGetInstance();

        static glm::mat3 m_currCameraMatrix;                ///< Current camera matrix
        static glm::mat3 m_currViewMatrix;
        static glm::mat3 m_currOrthoMatrix;

   

        static float m_currCameraRotate;
        static float m_currCameraScaleX;
        static float m_currCameraScaleY;
        static float m_currCameraTranslateX;
        static float m_currCameraTranslateY;

        static std::vector<glm::mat3> m_cameras;

    private:
        static std::unique_ptr<GraphicsCamera> m_instancePtr;
	};
}

#endif GRAPHCAM_H