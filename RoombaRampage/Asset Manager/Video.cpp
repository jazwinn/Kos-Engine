#include "../Config/pch.h"
#include "Video.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"






namespace video {

    Video::Video(std::string filepath, GLuint shaderProgram, std::bitset<VIDEO_FLAGS::TOTAL> flag ) : m_shaderProgram{ shaderProgram }
	{
		//graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();
		mpeg = plm_create_with_filename(filepath.c_str());

        //check flags 
        if (flag.test(VIDEO_FLAGS::AUDIO)) {
            plm_set_audio_enabled(mpeg, true);
        }
        if (flag.test(VIDEO_FLAGS::LOOP)) {
            plm_set_loop(mpeg, true);
        }


        videoWidth = plm_get_width(mpeg);
        videoHieght = plm_get_height(mpeg);

        glGenTextures(1, &yTexture);
        glGenTextures(1, &uTexture);
        glGenTextures(1, &vTexture);

        glBindTexture(GL_TEXTURE_2D, yTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth, videoHieght, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, uTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth / 2, videoHieght / 2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth / 2, videoHieght / 2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL init texture Error: " << std::hex << err << std::dec << " - ";
            switch (err) {
            case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
            default: std::cerr << "Unknown Error"; break;
            }
            std::cerr << std::endl;
        }

        glUseProgram(shaderProgram);
        glUniform1i(glGetUniformLocation(shaderProgram, "yTexture"), 0); // Bind to texture unit 0
        glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 1); // Bind to texture unit 1
        glUniform1i(glGetUniformLocation(shaderProgram, "vTexture"), 2); // Bind to texture unit 2
        glUseProgram(0);
	}

    

    Video::~Video()
    {
        glDeleteTextures(1, &yTexture);
        glDeleteTextures(1, &uTexture);
        glDeleteTextures(1, &vTexture);
        plm_destroy(mpeg);
    }

	void Video::DecodeAndUpdateVideo()
	{
        plm_frame_t* frame = plm_decode_video(mpeg);
        if (frame) {
            UpdateTextures(frame);
        }
	}

    void Video::DrawVideo(GLuint shaderProgram, GLuint quadVAO)
    {
        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, yTexture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, uTexture);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, vTexture);

        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "view"), 0, GL_FALSE, glm::value_ptr(graphicpipe::GraphicsCamera::m_currViewMatrix));
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, "projection"), 0, GL_FALSE, glm::value_ptr(graphicpipe::GraphicsCamera::m_currOrthoMatrix));

        glBindVertexArray(quadVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    void Video::UpdateTextures(plm_frame_t* frame)
    {
        if (!frame || !frame->y.data || !frame->cb.data || !frame->cr.data) {
            std::cerr << "Invalid frame data!" << std::endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, yTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width, frame->height, GL_RED, GL_UNSIGNED_BYTE, frame->y.data);

        glBindTexture(GL_TEXTURE_2D, uTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width / 2, frame->height / 2, GL_RED, GL_UNSIGNED_BYTE, frame->cb.data);

        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width / 2, frame->height / 2, GL_RED, GL_UNSIGNED_BYTE, frame->cr.data);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << std::hex << err << std::dec << " - ";
            switch (err) {
            case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
            default: std::cerr << "Unknown Error"; break;
            }
            std::cerr << std::endl;
        }

    }




}