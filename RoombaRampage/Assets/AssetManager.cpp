#include "AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "Prefab.h"
#include "../De&Serialization/json_handler.h"

#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/fwd.h"
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace assetmanager {


    std::unique_ptr<AssetManager> AssetManager::m_instancePtr = nullptr;


    void AssetManager::m_funcLoadAssets()
    {
        m_funcLoadImage("Assets/testBackground.png");
        m_funcLoadImage("Assets/blackTile_test.png");
        m_funcLoadImage("Assets/roombaTest.png");
        m_funcLoadImage("Assets/roombaTest2.png");
        m_funcLoadImage("Assets/roombaTest3.png");
        m_funcLoadImage("Assets/sprAssassinBar_strip8.png");

        m_LoadAudio("Assets/vacuum.mp3");
        m_LoadAudio("Assets/zwing.wav");

        prefab::Prefab::m_DeSerializePrefab("../RoombaRampage/Json/Prefab.json");

        //m_LoadFont("Assets/Roboto-Black.ttf", 48);
        m_LoadFont("Assets/AfacadFlux-Thin.ttf", 48);

    }

    AssetManager* AssetManager::m_funcGetInstance()
    {
        if (!m_instancePtr)
        {
            m_instancePtr.reset(new AssetManager{});
        }
        return m_instancePtr.get();
    }

    AssetManager::~AssetManager()
    {
        for (int i = 0; i < m_imageContainer.size(); ++i)
        {
            if (m_imagedataArray[i])
            {
                if (m_imageContainer[i].m_isPadded)
                {
                    delete m_imagedataArray[i];
                }
                else
                {
                    stbi_image_free(m_imagedataArray[i]);
                }
            }
        }
    }

   void AssetManager::m_LoadAudio(std::string file) {
        std::unique_ptr<fmodaudio::FModAudio> Audio = std::make_unique<fmodaudio::FModAudio>();
        Audio->m_init();
        Audio->m_createSound(file.c_str());

        m_audioContainer.push_back(std::move(Audio));
        Audio = nullptr;
        
    }
   void AssetManager::m_LoadFont(std::string file, unsigned int fontsize)
   {
       graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();

      
       FT_Library ft;
       // All functions return a value different than 0 whenever an error occurred
       if (FT_Init_FreeType(&ft))
       {
           std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
           return;
       }

       // find path to font
  
       if (file.empty())
       {
           std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
           return;
       }

       // load font as face
       FT_Face face;
       if (FT_New_Face(ft, file.c_str(), 0, &face)) {
           std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
           return;
       }
       else {
           // set size to load glyphs as
           FT_Set_Pixel_Sizes(face, 0, 48);

           
           // disable byte-alignment restriction
           glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

           

           // load first 128 characters of ASCII set
           for (unsigned char c = 0; c < 128; c++)
           {
               // Load character glyph 
               if (FT_Load_Char(face, c, FT_LOAD_RENDER))
               {
                   std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                   continue;
               }
               // generate texture
               unsigned int texture;
               glGenTextures(1, &texture);
               glBindTexture(GL_TEXTURE_2D, texture);

               glTexImage2D(
                   GL_TEXTURE_2D,
                   0,
                   GL_RED,
                   face->glyph->bitmap.width,
                   face->glyph->bitmap.rows,
                   0,
                   GL_RED,
                   GL_UNSIGNED_BYTE,
                   face->glyph->bitmap.buffer
               );
               // set texture options
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
               glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
               // now store character for later use
               graphicpipe::CharacterData character = {
                   texture,
                   glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                   static_cast<unsigned int>(face->glyph->advance.x)
               };
               graphics->m_characters.insert(std::pair<char, graphicpipe::CharacterData>(c, character));
           }
           glBindTexture(GL_TEXTURE_2D, 0);
       }
       // destroy FreeType once we're finished
       FT_Done_Face(face);
       FT_Done_FreeType(ft);
   }
 

    void AssetManager::m_funcLoadImage(const char* file)
    {
        graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();


        stbi_set_flip_vertically_on_load(true);
        Image image{};
        image.m_stripCount = m_extractStripCountFromFilename(file);

        image.m_spriteName = m_extractSpriteNameFromFilename(file);



        unsigned char* data = stbi_load(file, &image.m_width, &image.m_height, &image.m_channels, 0);
        if (!data)
        {
            std::cout << "Error: Could not load image: " << file << std::endl;
            return;
        }

        if (image.m_channels != m_targetChannels)
        {
            std::cout << "Warning: Color channels for " << file << " are not following RGBA specifications" << std::endl;
        }

        if (image.m_stripCount == 1)
        {
            if (image.m_width != image.m_height)
            {
                int targetHeight{};
                int targetWidth{};
                if (image.m_width > image.m_height)
                {
                    targetHeight = image.m_width;
                    targetWidth = image.m_width;
                }
                else
                {
                    targetHeight = image.m_height;
                    targetWidth = image.m_height;
                }
                unsigned char* newData = m_funcPadTexture(data, image.m_width, image.m_height, image.m_channels, targetWidth, targetHeight, m_targetChannels);
                stbi_image_free(data);
                image.m_isPadded = true;
                image.m_imageID = m_imageCount;
                image.m_width = targetWidth;
                image.m_height = targetHeight;
                image.m_channels = m_targetChannels;
                m_imageCount++;
                m_imageContainer.push_back(image);
                m_imagedataArray.push_back(newData);
                std::cout << "Texture Padded" << std::endl;
            }
            else
            {
                image.m_imageID = m_imageCount;
                m_imageCount++;
                m_imageContainer.push_back(image);
                m_imagedataArray.push_back(data);
            }
        }
        else
        {
            image.m_imageID = m_imageCount;
            m_imageCount++;
            m_imageContainer.push_back(image);
            m_imagedataArray.push_back(data);
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageContainer.back().m_width, m_imageContainer.back().m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imagedataArray.back());
        glGenerateMipmap(GL_TEXTURE_2D);
        graphics->m_textureIDs.push_back(textureID);
        std::cout << "Texture Binded, Texture ID: " << textureID << std::endl;

        graphics->m_imageData.push_back(m_imageContainer.back());

    }
    //TOCHECK
    //Naming of targetchannels is the same as the member variables
    unsigned char* AssetManager::m_funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels)
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

        // Calculate the maximum valid indices for source and destination arrays
        int maxSrcIndex = originalHeight * originalWidth * originalChannels;
        int maxDstIndex = targetHeight * targetWidth * targetChannels;

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
                    //paddedPixels[dstIndex] = originalPixels[srcIndex];
                                // Ensure indices are within bounds
                    if (srcIndex < maxSrcIndex && dstIndex < maxDstIndex)
                    {
                        // Copy pixel data
                        paddedPixels[dstIndex] = originalPixels[srcIndex];
                    }
                    else
                    {
                        // Handle out-of-bounds cases (e.g., log an error or continue)
                        // TOCHECK: You can add logging or handling logic here
                        LOGGING_ERROR("Buffer Overflow as SrcIndx > MaxSrcIndex || dstIndex > maxDstIndex");
                    }

                }
            }
        }

        return paddedPixels;
    }

    int AssetManager::m_extractStripCountFromFilename(const std::string& filename)
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

    std::string AssetManager::m_extractSpriteNameFromFilename(const std::string& filename)
    {
        //TOCHECK
        std::regex pattern("([[:alnum:]]+)([_]*)([[:alnum:]]*)\\.png");
        std::smatch match;

        if (std::regex_search(filename, match, pattern))
        {
            return match[1].str();
        }
        return "Error_Cannot_Read_Sprite_Name";
    }

    void AssetManager::m_serializeToJson(const std::string& filename) {
        rapidjson::Document document;
        document.SetObject();

        // Serialize imageContainer
        rapidjson::Value images(rapidjson::kArrayType);
        for (const auto& image : m_imageContainer) {
            rapidjson::Value imageJson(rapidjson::kObjectType);
            imageJson.AddMember("spriteName", rapidjson::Value(image.m_spriteName.c_str(), document.GetAllocator()), document.GetAllocator());
            imageJson.AddMember("stripCount", image.m_stripCount, document.GetAllocator());
            imageJson.AddMember("width", image.m_width, document.GetAllocator());
            imageJson.AddMember("height", image.m_height, document.GetAllocator());
            imageJson.AddMember("channels", image.m_channels, document.GetAllocator());
            imageJson.AddMember("isPadded", image.m_isPadded, document.GetAllocator());
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


    void AssetManager::m_deserializeFromJson(const std::string& filename) {
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
            image.m_spriteName = imageJson["spriteName"].GetString();
            image.m_stripCount = imageJson["stripCount"].GetInt();
            image.m_width = imageJson["width"].GetInt();
            image.m_height = imageJson["height"].GetInt();
            image.m_channels = imageJson["channels"].GetInt();
            image.m_isPadded = imageJson["isPadded"].GetBool();

            // Load image data from file (Right now assuming the image files are in the same directory) - Check with Jaz Win/Sean
            std::string imagePath = "Assets/" + image.m_spriteName + "_strip" + std::to_string(image.m_stripCount) + ".png";
            unsigned char* data = stbi_load(imagePath.c_str(), &image.m_width, &image.m_height, &image.m_channels, 0);
            if (!data) {
                std::cout << "Error: Could not load image: " << imagePath << std::endl;
                return;
            }

            m_imageContainer.push_back(image);
            m_imagedataArray.push_back(data);
        }
    }

    void AssetManager::m_testJSON() {

        // Create an instance of the AssetManager -> Already created in the app cpp thoo
        // AssetManager* assetManager = AssetManager::funcGetInstance();

        // Load some images (Also  loaded in app cpp)
        // assetManager->funcLoadImage("Assets/roombaTest.png");
        // assetManager->funcLoadImage("Assets/roombaTest2.png");

        // Serialize the image data to a JSON file
        this->m_serializeToJson("assets.json");

        // Clear the image container
        //this->imageContainer.clear();
        //this->imagedataArray.clear();

        // Deserialize the image data from the JSON file
        this->m_deserializeFromJson("assets.json");

        // Print the deserialized image data to test if the info was passed correctly
        for (const auto& image : this->m_imageContainer) {
            std::cout << "Sprite Name: " << image.m_spriteName << std::endl;
            std::cout << "Strip Count: " << image.m_stripCount << std::endl;
            std::cout << "Width: " << image.m_width << std::endl;
            std::cout << "Height: " << image.m_height << std::endl;
            std::cout << "Channels: " << image.m_channels << std::endl;
            std::cout << "Is Padded: " << image.m_isPadded << std::endl;
            std::cout << std::endl;
        }

        return;
    }

    void AssetManager::m_loadEntities(const std::string& jsonFilePath, ecs::ECS* ecs, std::vector<std::string>& obj_text_entries, std::vector<bool>& deleteButton, std::vector<bool>& DuplicateButton, std::vector<unsigned int>& obj_entity_id)
    {
        // Ensure the JSON file exists
        std::ifstream checkFile(jsonFilePath);
        if (!checkFile) {
            std::cerr << "Error: JSON file not found: " << jsonFilePath << std::endl;
            return;
        }

        // Load entities from the JSON file
        std::cout << "Loading entities from: " << jsonFilePath << std::endl;
        Serialization::Serialize::LoadComponentsJson(jsonFilePath, ecs, obj_text_entries);  // Load into ECS

        std::cout << "Entities successfully loaded!" << std::endl;
    }
}