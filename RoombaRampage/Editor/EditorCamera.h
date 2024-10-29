#ifndef EDITORCAM_H
#define EDITORCAM_H

#include <glm.hpp>
#include <vector>

    class EditorCamera
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
            float upVector;
            float rightVector;
            glm::vec2 m_coordinates;   ///< Coordinates of the camera.
            glm::vec2 m_zoom;          ///< Zoom of the a=canera
            float m_angle;             ///< Rotation angle of the camera.
        };

        static Camera m_editorCamera;
        static glm::mat3 m_editorCameraMatrix;
        static glm::mat3 m_editorViewMatrix;
        static glm::mat3 m_editorOrthoMatrix;
        static float m_editorCameraDragSensitivity;  ///< Editor Camera Drag Sensitivity
        static float m_editorCameraZoomSensitivity;  ///< Editor Camera Zoom Sensitivity

        static void calculateLevelEditorCamera();
        static void calculateLevelEditorView();
        static void calculateLevelEditorOrtho();



    };


#endif GRAPHCAM_H