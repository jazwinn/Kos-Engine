#ifndef ASSETMGR_H
#define ASSETMGR_H

#include <../Freetype_Font/include/ft2build.h>
#include <../Freetype_Font/include/freetype/freetype.h>
#include <../GLEW/include/GL/glew.h>
#include "../Assets/Audio.h"
#include "../ECS/ECS.h"

//#include "../Font.h"
#include <../Math/vector2.h>
#include <vector>
#include <map>
#include <string>
#include <memory>


namespace assetmanager {
	class AssetManager
	{
	private:
		//Access Point
		static std::unique_ptr<AssetManager> m_instancePtr;


		//Image channels required for our engine
		int m_targetChannels{ 4 };

		int m_imageCount{ 0 };

		unsigned char* m_funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels);
		int m_extractStripCountFromFilename(const std::string& filename);
		std::string m_extractSpriteNameFromFilename(const std::string& filename);


	public:
		enum
		{
			SPRITE_TEST
		};
		struct Image
		{
			std::string m_spriteName{};
			int m_width{};
			int m_height{};
			int m_channels{};
			int m_stripCount{};
			bool m_isPadded = false;
			int m_imageID{};
		};

		static AssetManager* m_funcGetInstance();
		~AssetManager();
		std::vector<Image> m_imageContainer;
		std::vector<unsigned char*> m_imagedataArray;
		std::vector<std::unique_ptr<fmodaudio::FModAudio>> m_audioContainer;
		void m_funcLoadAssets();
		void m_funcLoadImage(const char* file);
		void m_LoadAudio(std::string file);
		void m_LoadFont(std::string file, unsigned int fontsize);

		//De|Serialisation using rapid json
		void m_serializeToJson(const std::string& filename); //Serialize 
		void m_deserializeFromJson(const std::string& filename);

		//Load entity data
		void m_loadEntities(const std::string& jsonFilePath, ecs::ECS* ecs,
			std::vector<std::string>& obj_text_entries,
			std::vector<bool>& deleteButton,
			std::vector<bool>& DuplicateButton,
			std::vector<unsigned int>& obj_entity_id);

		//Test Code to test in application.cpp
		void m_testJSON();
	};
}


#endif ASSETMGR_H