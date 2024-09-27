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
    funcLoadImage("Assets/roombaTest.png");
    funcLoadImage("Assets/roombaTest2.png");
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
   // delete instancePtr;
}

void AssetManager::funcLoadImage(const char* file)
{
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
        if (image.width < targetWidth || image.height < targetHeight)
        {
            unsigned char* newData = funcPadTexture(data, image.width, image.height, image.channels);
            stbi_image_free(data);
            image.isPadded = true;
            image.width = targetWidth;
            image.height = targetHeight;
            
            image.imageID = imageCount;
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

unsigned char* AssetManager::funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels)
{
    unsigned char* paddedPixels = new unsigned char[targetWidth * targetHeight * targetChannels];

    // Fill with padding (e.g., transparent for RGBA)
    memset(paddedPixels, 0, targetWidth * targetHeight * targetChannels);

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
    std::regex pattern("(\\w)_strip(\\d+)\\.png");
    std::smatch match;

    if (std::regex_search(filename, match, pattern)) 
    {
        return std::stoi(match[1].str());
    }

    // Default to 1 if no number is found
    return 1;
}

std::string AssetManager::extractSpriteNameFromFilename(const std::string& filename)
{
    std::regex pattern("(\\w+)_.*\\.png");
    std::smatch match;

    if (std::regex_search(filename, match, pattern))
    {
        return match[1].str();
    }
    return "Error_Cannot_Read_Sprite_Name";
}


//GLuint loadPaddedTexture(const char* imagePath, int targetWidth, int targetHeight)
//{
//    // Load the image using stb_image or any other image-loading library
//    int width, height, channels;
//    unsigned char* data = stbi_load(imagePath, &width, &height, &channels, 0);
//    if (!data) {
//        std::cerr << "Failed to load image: " << imagePath << std::endl;
//        return 0;
//    }
//
//    // Create padded texture if necessary
//    unsigned char* paddedData = nullptr;
//    if (width != targetWidth || height != targetHeight) {
//        paddedData = padTexture(data, width, height, channels, targetWidth, targetHeight, channels);
//    }
//    else {
//        paddedData = data; // No need to pad if the sizes match
//    }
//
//    // Generate OpenGL texture
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // Set texture parameters (wrap and filter)
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//    // Upload the padded texture data to the GPU
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, targetWidth, targetHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, paddedData);
//
//    // Free original and padded data
//    if (data != paddedData) {
//        delete[] paddedData;
//    }
//    stbi_image_free(data);
//
//    return textureID;
//}