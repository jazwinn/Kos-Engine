#ifndef ASSETMGR_H
#define ASSETMGR_H


#include <../GLEW/include/GL/glew.h>
#include "../Assets/Audio.h"
#include "../ECS/ECS.h"
#include "Prefab.h"
#include "Image.h"
#include "Text.h"

//#include "../Font.h"
#include <../Math/Vector2.h>
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

		using Font = std::map<char, text::CharacterData>;
		

		std::vector<image::Image> m_imageContainer;
		std::vector<unsigned char*> m_imagedataArray;
		std::vector<std::unique_ptr<fmodaudio::FModAudio>> m_audioContainer;
		std::unordered_map<std::string, prefab::Prefab> m_prefabs;
		Font m_characters{};

		void m_funcLoadAssets();
		void m_funcLoadImage(const char* file);
		void m_LoadAudio(std::string file);
		void m_LoadFont(std::string file);
		void m_LoadPrefab(std::string file);

		//Load entity data
		void m_loadEntities(const std::string& jsonFilePath);


	};
}


#endif ASSETMGR_H