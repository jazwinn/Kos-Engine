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
        m_funcLoadImage("Assets/roombaBlink_strip4.png");
        m_funcLoadImage("Assets/roombaBlinkSmall_strip4.png");

        m_LoadAudio("Assets/vacuum.mp3");
        m_LoadAudio("Assets/zwing.wav");
        m_LoadPrefab("../RoombaRampage/Json/Prefab.json");
    
        m_LoadFont("Assets/AfacadFlux-Thin.ttf");

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

    void AssetManager::m_LoadPrefab(std::string file) {
        prefab::Prefab::m_DeSerializePrefab(file);

    }

   void AssetManager::m_LoadAudio(std::string file) {
        std::unique_ptr<fmodaudio::FModAudio> Audio = std::make_unique<fmodaudio::FModAudio>();
        Audio->m_init();
        Audio->m_createSound(file.c_str());

        m_audioContainer.push_back(std::move(Audio));
        Audio = nullptr;
        
    }
   void AssetManager::m_LoadFont(std::string file)
   {
       text::Font::LoadFont(file);
   }
 

    void AssetManager::m_funcLoadImage(const char* file)
    {
        graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();

        unsigned int textureID = image::Image::m_LoadImage(file);
       
        graphics->m_textureIDs.push_back(textureID);
        std::cout << "Texture Binded, Texture ID: " << textureID << std::endl;
        graphics->m_stripCounts.push_back(m_imageContainer.back().m_stripCount);
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
        Serialization::Serialize::m_LoadComponentsJson(jsonFilePath);  // Load into ECS

        std::cout << "Entities successfully loaded!" << std::endl;
    }
}