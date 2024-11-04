/******************************************************************/
/*!
\file      Helper.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the definations for the prefab class.
           It reads a json file and stores all its data. When the prefab
           is called in the game. It creates an entiy and copy
           the prefab data to the new entity



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Prefab.h"
#include "../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"
#include "AssetManager.h"
#include "../ECS/Hierachy.h"
#include "../ECS/ECS.h"
#include "SceneManager.h"
#include "../De&Serialization/json_handler.h"

#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/filewritestream.h"
#include "../Dependencies/rapidjson/prettywriter.h"


namespace prefab {

    void AssignPrefabToNameComponent(ecs::EntityID parentid, std::string scenename) {
        const auto& vecChild = ecs::Hierachy::m_GetChild(parentid);
        if (!vecChild.has_value()) return;
        for (auto& childid : vecChild.value()) {
            ecs::ECS* ecs = ecs::ECS::m_GetInstance();
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(childid));
            nc->m_isPrefab = true;
            nc->m_prefabName = scenename;

            if (ecs::Hierachy::m_GetChild(childid).has_value()) {
                AssignPrefabToNameComponent(childid, scenename);
            }
        }
    }

    void Prefab::m_CreatePrefab(std::string prefabscene, std::string insertscene)
    {
        if (prefabscene == insertscene) {
            LOGGING_ERROR("Cannot load onto itself");
            return;
        }



        //check if prefabscene exist
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        if (insertscene.empty()) {
            insertscene = ecs->m_ECS_SceneMap.begin()->first;
        }

        if (ecs->m_ECS_SceneMap.find(prefabscene) == ecs->m_ECS_SceneMap.end()) {
            LOGGING_ERROR("Prefab not loaded into scene");
            return;
        }
       
        std::vector<ecs::EntityID> vecid;

        for (const auto& prefabentity : ecs->m_ECS_SceneMap.find(prefabscene)->second.m_sceneIDs) {
            //duplicate only parents
            if (ecs::Hierachy::m_GetParent(prefabentity).has_value())continue;
            vecid.push_back(ecs->m_DuplicateEntity(prefabentity, insertscene));
        }

        for (auto& id : vecid) {
            //assign parent as prefab
            ecs::ECS* ecs = ecs::ECS::m_GetInstance();
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
            nc->m_isPrefab = true;
            nc->m_prefabName = insertscene;

            const auto& vecChild = ecs::Hierachy::m_GetChild(id);
            if (!vecChild.has_value()) continue;
            AssignPrefabToNameComponent(id, insertscene);
        
        }


    }

    void Prefab::m_SaveEntitytoPrefab(ecs::EntityID id)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(id));
        std::string m_jsonFilePath{ "../RoombaRampage/Assets/Prefabs/" }; //TODO allow drag and drop onto content browser

        std::string filename;

        short count{};
        do {
            if (count > 0) {
                filename = nc->m_entityName + "_" + std::to_string(count) + ".prefab";
            }
            else {
                filename = nc->m_entityName + ".prefab";
            }
            count++;
        } while (ecs->m_ECS_SceneMap.find(filename) != ecs->m_ECS_SceneMap.end());

        std::string path = m_jsonFilePath + filename;
        scenes::SceneManager::m_GetInstance()->m_CreateNewScene(path);


        /*******************************SERIALIZATION START******************************************/

        Serialization::Serialize::m_JsonFileValidation(path);

        // Create JSON object to hold the updated values
        rapidjson::Document doc;
        doc.SetArray();  // Initialize as an empty array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        std::unordered_set<ecs::EntityID> savedEntities;  //track saved entities

        //Start saving the entities
        Serialization::Serialize::m_SaveEntity(id, doc, allocator, savedEntities);

        // Write the JSON back to file
        rapidjson::StringBuffer writeBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
        doc.Accept(writer);

        std::ofstream outputFile(path);
        if (outputFile) {
            outputFile << writeBuffer.GetString();
            outputFile.close();
        }

        LOGGING_INFO("Save Prefab Successful");

        /*******************************SERIALIZATION END******************************************/


        // load prefab
        scenes::SceneManager::m_GetInstance()->m_LoadScene(path);


    }

}