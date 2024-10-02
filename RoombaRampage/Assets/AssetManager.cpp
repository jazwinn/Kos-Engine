#include "AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"

#include "stb_image.h"

#include "../De&Serialization/json_handler.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>




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

        unsigned int textureID = image::Image::m_LoadImage(file);
       
        graphics->m_textureIDs.push_back(textureID);
        std::cout << "Texture Binded, Texture ID: " << textureID << std::endl;

        graphics->m_imageData.push_back(m_imageContainer.back());

    }
   







    void AssetManager::m_loadEntities(const std::string& jsonFilePath)
    {
        // Ensure the JSON file exists
        std::ifstream checkFile(jsonFilePath);
        if (!checkFile) {
            std::cerr << "Error: JSON file not found: " << jsonFilePath << std::endl;
            return;
        }

        // Load entities from the JSON file
        std::cout << "Loading entities from: " << jsonFilePath << std::endl;
        Serialization::Serialize::LoadComponentsJson(jsonFilePath);  // Load into ECS

        std::cout << "Entities successfully loaded!" << std::endl;
    }
}