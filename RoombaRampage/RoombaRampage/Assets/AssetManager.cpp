#include "AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "Prefab.h"

#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/fwd.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace assetmanager {


    std::unique_ptr<AssetManager> AssetManager::instancePtr = nullptr;


    void AssetManager::funcLoadAssets()
    {

        funcLoadImage("Assets/blackTile_test.png");
        funcLoadImage("Assets/testBackground.png");
        funcLoadImage("Assets/roombaTest.png");
        funcLoadImage("Assets/roombaTest2.png");
        funcLoadImage("Assets/roombaTest3.png");
        funcLoadImage("Assets/sprAssassinBar_strip8.png");

        m_LoadAudio("Assets/vacuum.mp3");
        m_LoadAudio("Assets/zwing.wav");

        prefab::Prefab::m_DeSerializePrefab("../RoombaRampage/Json/Prefab.json");

        //m_LoadFont("Assets/Roboto-Black.ttf", 12);

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
        for (int i = 0; i < imageContainer.size(); ++i)
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

   void AssetManager::m_LoadAudio(std::string file) {
        std::unique_ptr<FModAudio> Audio = std::make_unique<FModAudio>();
        Audio->init();
        Audio->createSound(file.c_str());

        m_audioContainer.push_back(std::move(Audio));
        Audio = nullptr;
        
    }
   // To implement
   /*void m_LoadFont(std::string file, unsigned int fontsize) {
       std::unique_ptr<fontmanager::Font> Font = std::make_unique<fontmanager::Font>();
       Font(file, fontsize);
   }*/

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

    void AssetManager::serializeToJson(const std::string& filename) {
        rapidjson::Document document;
        document.SetObject();

        // Serialize imageContainer
        rapidjson::Value images(rapidjson::kArrayType);
        for (const auto& image : imageContainer) {
            rapidjson::Value imageJson(rapidjson::kObjectType);
            imageJson.AddMember("spriteName", rapidjson::Value(image.spriteName.c_str(), document.GetAllocator()), document.GetAllocator());
            imageJson.AddMember("stripCount", image.stripCount, document.GetAllocator());
            imageJson.AddMember("width", image.width, document.GetAllocator());
            imageJson.AddMember("height", image.height, document.GetAllocator());
            imageJson.AddMember("channels", image.channels, document.GetAllocator());
            imageJson.AddMember("isPadded", image.isPadded, document.GetAllocator());
            images.PushBack(imageJson, document.GetAllocator());
        }
        document.AddMember("images", images, document.GetAllocator());

        // Write to file
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        std::ofstream file(filename);
        file << buffer.GetString();
        file.close();
    }


    void AssetManager::deserializeFromJson(const std::string& filename) {
        // Read from file
        std::ifstream file(filename);
        std::string json((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // Parse the JSON string
        rapidjson::Document document;
        document.Parse(json.c_str());

        // Check for parsing errors
        if (document.HasParseError()) {
            std::cerr << "Error parsing JSON: " << document.GetParseError() << std::endl;
            return;
        }

        // Deserialize imageContainer
        const rapidjson::Value& images = document["images"];
        for (rapidjson::SizeType i = 0; i < images.Size(); ++i) {
            const rapidjson::Value& imageJson = images[i];
            Image image;
            image.spriteName = imageJson["spriteName"].GetString();
            image.stripCount = imageJson["stripCount"].GetInt();
            image.width = imageJson["width"].GetInt();
            image.height = imageJson["height"].GetInt();
            image.channels = imageJson["channels"].GetInt();
            image.isPadded = imageJson["isPadded"].GetBool();

            // Load image data from file (Right now assuming the image files are in the same directory) - Check with Jaz Win/Sean
            std::string imagePath = "Assets/" + image.spriteName + "_strip" + std::to_string(image.stripCount) + ".png";
            unsigned char* data = stbi_load(imagePath.c_str(), &image.width, &image.height, &image.channels, 0);
            if (!data) {
                std::cout << "Error: Could not load image: " << imagePath << std::endl;
                return;
            }

            imageContainer.push_back(image);
            imagedataArray.push_back(data);
        }
    }

    void AssetManager::testJSON() {

        // Create an instance of the AssetManager -> Already created in the app cpp thoo
        // AssetManager* assetManager = AssetManager::funcGetInstance();

        // Load some images (Also  loaded in app cpp)
        // assetManager->funcLoadImage("Assets/roombaTest.png");
        // assetManager->funcLoadImage("Assets/roombaTest2.png");

        // Serialize the image data to a JSON file
        this->serializeToJson("assets.json");

        // Clear the image container
        //this->imageContainer.clear();
        //this->imagedataArray.clear();

        // Deserialize the image data from the JSON file
        this->deserializeFromJson("assets.json");

        // Print the deserialized image data to test if the info was passed correctly
        for (const auto& image : this->imageContainer) {
            std::cout << "Sprite Name: " << image.spriteName << std::endl;
            std::cout << "Strip Count: " << image.stripCount << std::endl;
            std::cout << "Width: " << image.width << std::endl;
            std::cout << "Height: " << image.height << std::endl;
            std::cout << "Channels: " << image.channels << std::endl;
            std::cout << "Is Padded: " << image.isPadded << std::endl;
            std::cout << std::endl;
        }

        return;
    }
}