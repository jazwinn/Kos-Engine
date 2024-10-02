#ifndef ASSETMGR_H
#define ASSETMGR_H

#include <../Freetype_Font/include/ft2build.h>
#include <../Freetype_Font/include/freetype/freetype.h>
#include <../GLEW/include/GL/glew.h>
#include "../Assets/Audio.h"
#include "../ECS/ECS.h"
#include "Prefab.h"
#include "Image.h"

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





	public:

		static AssetManager* m_funcGetInstance();
		~AssetManager();

		std::vector<image::Image> m_imageContainer;
		std::vector<unsigned char*> m_imagedataArray;
		std::vector<std::unique_ptr<fmodaudio::FModAudio>> m_audioContainer;
		std::unordered_map<std::string, prefab::Prefab> m_prefabs;


		void m_funcLoadAssets();
		void m_funcLoadImage(const char* file);
		void m_LoadAudio(std::string file);
		void m_LoadFont(std::string file, unsigned int fontsize);

		//De|Serialisation using rapid json
		void m_serializeToJson(const std::string& filename); //Serialize 
		void m_deserializeFromJson(const std::string& filename);

		//Load entity data
		void m_loadEntities(const std::string& jsonFilePath);

		//Test Code to test in application.cpp
		void m_testJSON();
	};
}


#endif ASSETMGR_H