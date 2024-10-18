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

        static bool m_editorMode;
        /**
         * @struct Camera
         * @brief Represents the camera's position and orientation.
         */
        struct Camera                  ///< ONLY MEANT FOR LEVEL EDITOR CAMERA
        {
            glm::vec2 m_coordinates;   ///< Coordinates of the camera.
            glm::vec2 m_zoom;          ///< Zoom of the a=canera
            float m_angle;             ///< Rotation angle of the camera.
        };

        static Camera m_editorCamera;
        static glm::mat3 m_editorCameraMatrix;
        static float m_editorCameraDragSensitivity;  ///< Editor Camera Drag Sensitivity
        static float m_editorCameraZoomSensitivity;  ///< Editor Camera Zoom Sensitivity

        static void calculateAspectRatio();
        static void multiplyActiveCameraMatrix();
        static void setActiveCamera(unsigned int index);
        static void setLevelEditorCamera();
        static void calculateLevelEditorCamera();

        static GraphicsCamera* m_funcGetInstance();

        static glm::mat3 m_currCameraMatrix;                ///< Current camera matrix
        static std::vector<glm::mat3> cameraMatrices;         ///< Container for camera components matrices

    private:
        static std::unique_ptr<GraphicsCamera> m_instancePtr;
	};
}

#endif GRAPHCAM_H