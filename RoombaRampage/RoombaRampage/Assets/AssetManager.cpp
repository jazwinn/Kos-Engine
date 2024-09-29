#include "AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

std::unique_ptr<AssetManager> AssetManager::instancePtr = nullptr;


void AssetManager::funcLoadAssets()
{
    
    funcLoadImage("Assets/blackTile_test.png");
    funcLoadImage("Assets/testBackground.png");
    funcLoadImage("Assets/roombaTest.png");
    funcLoadImage("Assets/roombaTest2.png");
    funcLoadImage("Assets/roombaTest3.png");
    funcLoadImage("Assets/sprAssassinBar_strip8.png");
}

AssetManager* AssetManager::funcGetInstance()
{
    if (!instancePtr) 
    {
        instancePtr.reset(new AssetManager{});
    }
    return instancePtr.get();
}

AssetManager::~AssetManager()
{
    for(int i = 0; i < imageContainer.size(); ++i)
    {
        if (imagedataArray[i])
        {
            if (imageContainer[i].isPadded)
            {
                delete imagedataArray[i];
            }
            else
            {
                stbi_image_free(imagedataArray[i]);
            }
        }
    }
}

void AssetManager::funcLoadImage(const char* file)
{
    stbi_set_flip_vertically_on_load(true);
	Image image{};
    image.stripCount = extractStripCountFromFilename(file);
   
    image.spriteName = extractSpriteNameFromFilename(file);
    
   
    
	unsigned char* data = stbi_load(file, &image.width, &image.height, &image.channels, 0);
    if (!data)
    {
        std::cout << "Error: Could not load image: " << file << std::endl;
        return;
    }

    if (image.channels != targetChannels)
    {
        std::cout << "Warning: Color channels for " << file << " are not following RGBA specifications" << std::endl;
    }

    if (image.stripCount == 1)
    {
        if (image.width != image.height)
        {
            int targetHeight{};
            int targetWidth{};
            if (image.width > image.height)
            {
                targetHeight = image.width;
                targetWidth = image.width;
            }
            else
            {
                targetHeight = image.height;
                targetWidth = image.height;
            }
            unsigned char* newData = funcPadTexture(data, image.width, image.height, image.channels, targetWidth, targetHeight, targetChannels);
            stbi_image_free(data);
            image.isPadded = true;      
            image.imageID = imageCount;
            image.width = targetWidth;
            image.height = targetHeight;
            image.channels = targetChannels;
            imageCount++;
            imageContainer.push_back(image);
            imagedataArray.push_back(newData);
            std::cout << "Texture Padded" << std::endl;
        }
        else
        {
            image.imageID = imageCount;
            imageCount++;
            imageContainer.push_back(image);
            imagedataArray.push_back(data);
        }
    }
    else
    {
        image.imageID = imageCount;
        imageCount++;
        imageContainer.push_back(image);
        imagedataArray.push_back(data);
    }
   
}
//TOCHECK
//Naming of targetchannels is the same as the member variables
unsigned char* AssetManager::funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels)
{
    unsigned char* paddedPixels = new unsigned char[targetWidth * targetHeight * targetChannels];

    // Fill with padding (e.g., transparent for RGBA)
    memset(paddedPixels, 0, targetWidth * targetHeight * targetChannels);

    //If the image doesn't have alpha channel
    if (targetChannels > originalChannels)
    {
        for (int y = 0; y < originalHeight; ++y)
        {
            for (int x = 0; x < originalWidth; ++x)
            {
                    int c = 3;
                    int dstIndex = ((y + (targetHeight - originalHeight) / 2) * targetWidth + (x + (targetWidth - originalWidth) / 2)) * targetChannels + c;

                    // Make Opaque
                    paddedPixels[dstIndex] = 255;
            }
        }
    }

    // Copy the original texture into the center of the padded texture
    for (int y = 0; y < originalHeight; ++y) 
    {
        for (int x = 0; x < originalWidth; ++x) 
        {
            for (int c = 0; c < originalChannels; ++c) 
            {
                // Calculate source and destination indices
                int srcIndex = (y * originalWidth + x) * originalChannels + c;
                int dstIndex = ((y + (targetHeight - originalHeight) / 2) * targetWidth + (x + (targetWidth - originalWidth) / 2)) * targetChannels + c;
                //TOCHECK
                // Copy pixel data
                paddedPixels[dstIndex] = originalPixels[srcIndex];
              
            }
        }
    }

    return paddedPixels;
}

int AssetManager::extractStripCountFromFilename(const std::string& filename) 
{
    // Use regex to find the strip count in the format "something_strip(number).png"
    std::regex pattern("([[:alnum:]])_strip([[:digit:]]+)\\.png");
    std::smatch match;

    if (std::regex_search(filename, match, pattern)) 
    {
        std::cout << "Strip Success" << std::endl;
        return std::stoi(match[2].str());
    }

    // Default to 1 if no number is found
    return 1;
}

std::string AssetManager::extractSpriteNameFromFilename(const std::string& filename)
{
    //TOCHECK
    std::regex pattern("([[:alnum:]]+)([_]*)([[:alnum:]]*)\.png");
    std::smatch match;

    if (std::regex_search(filename, match, pattern))
    {
        return match[1].str();
    }
    return "Error_Cannot_Read_Sprite_Name";
}
