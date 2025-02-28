#ifndef VIDEO_H
#define VIDEO_H

#include <glew.h>
#include <glfw3.h>

#include "../Config/pch.h"

#include "pl_mpeg.h"

namespace video {

	enum VIDEO_FLAGS {
		AUDIO,
		LOOP,
		TOTAL
	};

	class Video
	{
	public:
		Video(std::string filepath, GLuint shaderProgram , std::bitset<VIDEO_FLAGS::TOTAL> FLAGS = 0);
		~Video();

	public:
		void DecodeAndUpdateVideo();

		void DrawVideo(GLuint shaderProgram, GLuint quadVAO);

	private:
		plm_t* mpeg;
		int videoWidth, videoHieght;

		void UpdateTextures(plm_frame_t* frame);

		GLuint yTexture, uTexture, vTexture;

		std::map<std::string, int> uniformLocations;

		GLuint uni_yTexture, uni_uTexture, uni_vTexture;

		GLuint& m_shaderProgram;
	};







}



#endif VIDEO_H



