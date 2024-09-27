#ifndef ASSETMGR_H
#define ASSETMGR_H
#include <vector>
#include <map>
#include <string>
#include <memory>

class AssetManager
{
private:
	//Access Point
	static std::unique_ptr<AssetManager> instancePtr;


	//Image dimensions required for our engine
	int targetWidth{ 1280 };
	int targetHeight{ 1280 };
	int targetChannels{ 4 };

	int imageCount{ 0 };
	
	unsigned char* funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels);
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

	void funcLoadAssets();
	void funcLoadImage(const char* file);

	
	
};

#endif ASSETMGR_H