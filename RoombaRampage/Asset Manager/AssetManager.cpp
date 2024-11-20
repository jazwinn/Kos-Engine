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
#include "SceneManager.h"
#include "stb_image.h"




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
            else {
                m_LoadAsset(directoryPath.path());
            }

        }

       
    }

    void AssetManager::m_LoadAsset(std::filesystem::path directoryPath)
    {
        std::string filepath = directoryPath.string();
        std::replace(filepath.begin(), filepath.end(), '\\', '/');

        if (directoryPath.filename().extension().string() == ".png") {
            std::cout << filepath.c_str() << std::endl;
            m_funcLoadImage(filepath);

        }
        else if (directoryPath.filename().extension().string() == ".wav") {

            m_LoadAudio(filepath.c_str());
        }
        else if (directoryPath.filename().extension().string() == ".ttf") {
            //UNCOMMENT ME TO TEST IT OUT
            m_LoadFont(filepath);
        }
        else if (directoryPath.filename().extension().string() == ".cs") {
            m_LoadScript(directoryPath);
        }
        if (directoryPath.filename().extension().string() == ".prefab") {

            scenes::SceneManager::m_GetInstance()->m_LoadScene(directoryPath);
        }
        if (directoryPath.filename().extension().string() == ".json") {

            scenes::SceneManager::m_GetInstance()->m_unloadScenePath.insert(std::pair{ directoryPath.filename().string(), directoryPath});

        }



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
       // prefab::Prefab::m_DeSerializePrefab(file);

    }

    void AssetManager::m_RenameAsset(std::filesystem::path oldfilepath, std::filesystem::path newfilepath)
    {
        if (oldfilepath.filename().extension().string() != newfilepath.filename().extension().string()) {
            LOGGING_WARN("Renaming assets of different type");
            return;
        }

        // find the file extension (Update for every new file type
        if (oldfilepath.extension().string() == ".png") {
            //check if old file name is present
            if (m_imageManager.m_imageMap.find(oldfilepath.filename().string()) != m_imageManager.m_imageMap.end()) {
                //check if new file name is non existent
                if (m_imageManager.m_imageMap.find(newfilepath.filename().string()) == m_imageManager.m_imageMap.end()) {

                    //create new map of updated name
                    m_imageManager.m_imageMap[newfilepath.filename().string()] = m_imageManager.m_imageMap.find(oldfilepath.filename().string())->second;                 
                    //delete old name map
                    m_imageManager.m_imageMap.erase(oldfilepath.filename().string());
                }
                else {
                    return;
                }
            }
            else {
                return;
            }
        }

        std::filesystem::rename(oldfilepath.c_str(), newfilepath.c_str());
    }

    void AssetManager::m_LoadScript(std::filesystem::path filepath)
    {
        //m_scriptManager.m_AddScripts(filepath.string());
        //std::string filename = filepath.filename().stem().string();
        //// load start and update
        ////TODO move somewhere that is not the asset manager
        //m_scriptManager.m_LoadMethod(filename, filename, "Start", 0);
        //m_scriptManager.m_LoadMethod(filename, filename, "Update", 0);

        std::string filename = filepath.filename().stem().string();
        
        if (std::find(m_scriptManager.m_CSScripts.begin(), m_scriptManager.m_CSScripts.end(), filename) == m_scriptManager.m_CSScripts.end()) {
            //store if filename is not inside
            m_scriptManager.m_CSScripts.push_back(filename);
        }

        

    }

   void AssetManager::m_LoadAudio(std::string file) {
       m_audioManager.m_LoadAudio(file); 
    }
   void AssetManager::m_LoadFont(std::string file)
   {
       text::FontManager::LoadFont(file);
   }
 

    void AssetManager::m_funcLoadImage(std::string file)
    {
        m_imageManager.m_LoadImage(file.c_str());
    }

  
}