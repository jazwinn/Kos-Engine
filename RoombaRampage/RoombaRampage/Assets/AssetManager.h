#ifndef ASSETMGR_H
#define ASSETMGR_H

#include "../Assets/Audio.h"
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
		static std::unique_ptr<AssetManager> instancePtr;


		//Image channels required for our engine
		int targetChannels{ 4 };

		int imageCount{ 0 };

		unsigned char* funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels);
		int extractStripCountFromFilename(const std::string& filename);
		std::string extractSpriteNameFromFilename(const std::string& filename);


	public:
		enum
		{
			SPRITE_TEST
		};
		struct Image
		{
			std::string spriteName;
			int width;
			int height;
			int channels;
			int stripCount;
			bool isPadded = false;
			int imageID;
		};



		static AssetManager* funcGetInstance();
		~AssetManager();
		std::vector<Image> imageContainer;
		std::vector<unsigned char*> imagedataArray;
		std::vector<std::unique_ptr<FModAudio>> m_audioContainer;

		void funcLoadAssets();
		void funcLoadImage(const char* file);
		void m_LoadAudio(std::string file);

		//void m_LoadFont(std::string file, unsigned int fontsize);

		//De|Serialisation using rapid json
		void serializeToJson(const std::string& filename); //Serialize 
		void deserializeFromJson(const std::string& filename);

		//Test Code to test in application.cpp
		void testJSON();
	};
}


#endif ASSETMGR_H