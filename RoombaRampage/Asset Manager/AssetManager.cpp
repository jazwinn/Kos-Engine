/******************************************************************/
/*!
\file      AssetManager.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 27, 2024
\brief     This file contains the definition of the AssetManager class,
           which handles loading, storing, and managing various game assets.

The AssetManager class provides functions for loading and managing
images, audio files, fonts, prefabs, and entity data. It serves as a
singleton, ensuring only one instance manages the resources for the
entire game. Assets are stored in containers for later retrieval and use.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"
#include "stb_image.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

namespace assetmanager {

    std::unique_ptr<AssetManager> AssetManager::m_instancePtr = nullptr;

    void AssetManager::m_funcLoadAssets(std::string Directory )
    {
        for (auto& directoryPath : std::filesystem::directory_iterator(Directory)) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            if (directoryPath.is_directory()) {
                m_funcLoadAssets(filepath);
            }
            else if (directoryPath.path().filename().extension().string() == ".png") {
                std::cout << filepath.c_str() << std::endl;
                m_funcLoadImage(filepath);
                
            }
            else if (directoryPath.path().filename().extension().string() == ".wav") {

                m_LoadAudio(filepath.c_str());
            }
            else if (directoryPath.path().filename().extension().string() == ".ttf") {
                //UNCOMMENT ME TO TEST IT OUT
                //m_LoadFont("Assets/Font/Roboto-Black.ttf");
            }
            

        }

        

        //m_funcLoadImage("Assets/Sprites/testBackground.png");
        //m_funcLoadImage("Assets/Sprites/blackTile_test.png");
        //m_funcLoadImage("Assets/Sprites/roombaTest.png");
        //m_funcLoadImage("Assets/Sprites/roombaTest2.png");
        //m_funcLoadImage("Assets/Sprites/roombaTest3.png");
        //m_funcLoadImage("Assets/Sprites/gunleft_02.png");
        //m_funcLoadImage("Assets/Sprites/gunright_02.png");
        //m_funcLoadImage("Assets/Sprites/boosters_tail02.png");

        //m_funcLoadImage("Assets/Sprites/ani_RoombaBlink_strip4.png");
        //m_funcLoadImage("Assets/Sprites/ani_RoombaBoosters_strip6.png");
        //m_funcLoadImage("Assets/Sprites/ani_SeanSprite_strip4.png");


        //m_LoadAudio("Assets/Audio/mindstorm.wav");
        //m_LoadAudio("Assets/Audio/zwing.wav");

        //m_LoadPrefab("../RoombaRampage/Assets/Prefabs/Prefab.json");

        //m_LoadFont("Assets/Font/Roboto-Black.ttf");
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
        
    }

    void AssetManager::m_LoadPrefab(std::string file) {
        prefab::Prefab::m_DeSerializePrefab(file);

    }

   void AssetManager::m_LoadAudio(std::string file) {
       m_audioManager.m_LoadAudio(file); 
    }
   void AssetManager::m_LoadFont(std::string file)
   {
       text::Font::LoadFont(file);
   }
 

    void AssetManager::m_funcLoadImage(std::string file)
    {
        m_imageManager.m_LoadImage(file.c_str());
    }

  
}