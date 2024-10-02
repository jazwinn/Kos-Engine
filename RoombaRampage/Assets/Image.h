#ifndef IMAGE_H
#define IMAGE_H

#include <string>


namespace image {

    class Image {
    public:
        static unsigned int m_LoadImage(const char* file);
		static int m_extractStripCountFromFilename(const std::string& filename);
		static std::string m_extractSpriteNameFromFilename(const std::string& filename);
		static unsigned char* m_funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels);
		static void m_serializeToJson(const std::string& filename);
		static void m_deserializeFromJson(const std::string& filename);
		static void m_testJSON();


		//Image channels required for our engine
		static int m_targetChannels;

		static int m_imageCount;

	public:
		std::string m_spriteName{};
		int m_width{};
		int m_height{};
		int m_channels{};
		int m_stripCount{};
		bool m_isPadded = false;
		int m_imageID{};
    };
}


#endif IMAGE_H
