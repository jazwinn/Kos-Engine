#ifndef VIDEO_H
#define VIDEO_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>

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

		void SetTransformation(glm::mat3 transform) {
			transformation = std::move(transform);
		}

		GLuint yTexture, uTexture, vTexture;

	private:
		void UpdateTextures(plm_frame_t* frame);

		plm_t* mpeg;

		glm::mat3 transformation;

		int videoWidth, videoHieght;
		

		std::map<std::string, int> uniformLocations;

		GLuint uni_yTexture, uni_uTexture, uni_vTexture;

		GLint locTransformation, locView, locProjection;

		GLuint m_shaderProgram;
	};


	class VideoManager {






	public:

		std::unordered_map<std::string, std::filesystem::path> m_videopath; //stores path of video

		//std::unordered_map<ecs::EntityID, std::unique_ptr<Video>> m_videoMap; // store all playing videos

	};




}



#endif VIDEO_H



