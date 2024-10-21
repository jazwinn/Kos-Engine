/******************************************************************/
/*!
\file      AssetManager.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 27, 2024
\brief     This file contains the declaration of the AssetManager class,
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

#ifndef ASSETMGR_H
#define ASSETMGR_H


#include <../GLEW/include/GL/glew.h>
#include "../Assets/Audio.h"
#include "../ECS/ECS.h"
#include "Prefab.h"
#include "Image.h"
#include "Text.h"
#include <../Math/vector2.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace assetmanager {

    /******************************************************************/
    /*!
    \class   AssetManager
    \brief   A singleton class that manages the loading, storage, and
             retrieval of game assets, including images, audio, fonts,
             and prefabs.
    */
    /******************************************************************/
    class AssetManager {
    private:
        /******************************************************************/
        /*!
        \var     static std::unique_ptr<AssetManager> m_instancePtr
        \brief   Unique pointer to the singleton instance of AssetManager.
        */
        /******************************************************************/
        static std::unique_ptr<AssetManager> m_instancePtr;

    public:
        /******************************************************************/
        /*!
        \fn      static AssetManager* m_funcGetInstance()
        \brief   Retrieves the singleton instance of AssetManager. If the
                 instance does not exist, it creates one.
        \return  Pointer to the singleton instance of AssetManager.
        */
        /******************************************************************/
        static AssetManager* m_funcGetInstance();

        /******************************************************************/
        /*!
        \fn      ~AssetManager()
        \brief   Destructor that cleans up any dynamically allocated assets.
        */
        /******************************************************************/
        ~AssetManager();

        using Font = std::map<char, text::CharacterData>;

        /******************************************************************/
        /*!
        \var     std::vector<image::Image> m_imageContainer
        \brief   Container for storing loaded image assets.
        */
        /******************************************************************/
        std::vector<image::Image> m_imageContainer;

        /******************************************************************/
        /*!
        \var     std::vector<unsigned char*> m_imagedataArray
        \brief   Array for storing raw image data pointers.
        */
        /******************************************************************/
        std::vector<unsigned char*> m_imagedataArray;

        /******************************************************************/
        /*!
        \var     std::vector<std::unique_ptr<fmodaudio::FModAudio>> m_audioContainer
        \brief   Container for storing loaded audio assets.
        */
        /******************************************************************/
        std::vector<std::unique_ptr<fmodaudio::FModAudio>> m_audioContainer;

        /******************************************************************/
        /*!
        \var     std::unordered_map<std::string, prefab::Prefab> m_prefabs
        \brief   Map storing prefab assets by string identifier.
        */
        /******************************************************************/
        std::unordered_map<std::string, prefab::Prefab> m_prefabs;

        /******************************************************************/
        /*!
        \var     Font m_characters
        \brief   Map storing font character data.
        */
        /******************************************************************/
        Font m_characters{};

        /******************************************************************/
        /*!
        \fn      void m_funcLoadAssets()
        \brief   Loads all necessary assets (images, audio, fonts, prefabs)
                 required by the game.
        */
        /******************************************************************/
        void m_funcLoadAssets();

        /******************************************************************/
        /*!
        \fn      void m_funcLoadImage(const char* file)
        \brief   Loads an image file and stores it in the image container.
        \param   file - Path to the image file to load.
        */
        /******************************************************************/
        void m_funcLoadImage(const char* file);

        /******************************************************************/
        /*!
        \fn      void m_LoadAudio(std::string file)
        \brief   Loads an audio file and stores it in the audio container.
        \param   file - Path to the audio file to load.
        */
        /******************************************************************/
        void m_LoadAudio(std::string file);

        /******************************************************************/
        /*!
        \fn      void m_LoadFont(std::string file)
        \brief   Loads a font file and stores character data in the font map.
        \param   file - Path to the font file to load.
        */
        /******************************************************************/
        void m_LoadFont(std::string file);

        /******************************************************************/
        /*!
        \fn      void m_LoadPrefab(std::string file)
        \brief   Loads a prefab file and stores it in the prefab map.
        \param   file - Path to the prefab file to load.
        */
        /******************************************************************/
        void m_LoadPrefab(std::string file);


    };

}

#endif // ASSETMGR_H