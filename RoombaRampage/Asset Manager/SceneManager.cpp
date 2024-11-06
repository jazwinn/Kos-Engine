#include "SceneManager.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filewritestream.h"
#include "../ECS/ECS.h"
#include "../ECS/Hierachy.h"
#include "Prefab.h"

namespace scenes {

    std::unique_ptr<SceneManager> SceneManager::m_InstancePtr = nullptr;


    bool SceneManager::m_CreateNewScene(std::filesystem::path scene)
    {
        std::ifstream checkFile(scene.string());
        //check if file name exist
        if (checkFile) {
            //if file name exist
            LOGGING_WARN("JSON file already exist, select another name");
            return false;
        }
        //create a json file

        FILE* fp = std::fopen(scene.string().c_str(), "wb");

        if (fp == nullptr) {
            LOGGING_ERROR("Fail to create new Scene");
        }

        // start with []
        fprintf(fp, "[]");
        char writeBuffer[1];  // Buffer to optimize file writing
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

        std::fclose(fp);

        // return file path
        return true;
    }

    void SceneManager::m_LoadScene(std::filesystem::path scene)
    {
        // check if it is json file type
        if (scene.filename().extension().string() != ".json" && scene.filename().extension().string() != ".prefab") {
            LOGGING_WARN("File Type not .json");
            return;
        }


        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        if (ecs->m_ECS_SceneMap.find(scene.filename().string()) != ecs->m_ECS_SceneMap.end()) {
#if _DEBUG
            LOGGING_ERROR("Scene already loaded");
#endif
            return;
        }

        // Ensure the JSON file exists

        std::ifstream checkFile(scene.string());
        if (!checkFile) {
            if (!m_CreateNewScene(scene)) {
                LOGGING_ERROR("Fail to Create file");
                return;
            }
   
        }

        //contain scene path
        m_scenePath[scene.filename().string()] = scene;
        
        // store path to be use as recent
        if (scene.filename().extension().string() != ".prefab") {

            if (std::find_if(m_recentFiles.begin(), m_recentFiles.end(),
                [&scene](const std::filesystem::path& path) {
                    return path.filename().string() == scene.filename().string();
                }) == m_recentFiles.end()) {

                // Add the scene to recent files if it's not already present
                m_recentFiles.push_back(scene);
            }
        }


        std::string scenename = scene.filename().string();

        //create new scene
        ecs->m_ECS_SceneMap[scenename];
        //check if file is prefab or scene

            
        // Load entities from the JSON file
        std::cout << "Loading entities from: " << scene.string() << std::endl;
        Serialization::Serialize::m_LoadComponentsJson(scene.string());  // Load into ECS

        if (scene.filename().extension().string() == ".prefab") {
            ecs->m_ECS_SceneMap.find(scenename)->second.m_isPrefab = true;
            ecs->m_ECS_SceneMap.find(scenename)->second.m_isActive = false;

            for (auto& id : ecs->m_ECS_SceneMap.find(scenename)->second.m_sceneIDs) {
                ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));
                if (!tc->m_haveParent) {
                    ecs->m_ECS_SceneMap.find(scenename)->second.m_prefabID = id;
                    break;
                }
            }
             


        }

        LOGGING_INFO("Entities successfully loaded!");

    }

    void SceneManager::m_ReloadScene()
    {
        //retrieve open scenes
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        std::vector<std::string> sce;
        for (auto& scenes : ecs->m_ECS_SceneMap) {
            sce.push_back(scenes.first);
        }

        //store scene path
        std::vector<std::string> scenepath;
        for (auto& scene : sce) {
            scenepath.push_back(m_scenePath.find(scene)->second.string());
        }
        
        //clear all scenes
        m_ClearAllScene();

        //load baack previous scene

        for (auto& scene : scenepath) {
            m_LoadScene(scene);
        }


    }

    void SceneManager::m_ClearAllScene() //EXCEPT PREFABS
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        std::vector<std::string> sce;
        for (auto& scenes : ecs->m_ECS_SceneMap) {
            //exclude prefabs
            if (scenes.second.m_isPrefab == true) continue;
            sce.push_back(scenes.first);
        }

        for (auto& scenes : sce) {
            m_ClearScene(scenes);
        }


    }

    void SceneManager::m_ClearScene(std::string scene)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        size_t numberOfEntityInScene = ecs->m_ECS_SceneMap.find(scene)->second.m_sceneIDs.size();
        for (int n{}; n < numberOfEntityInScene; n++) {
            if (ecs->m_ECS_SceneMap.find(scene)->second.m_sceneIDs.size() <= 0) break;
            auto entityid = ecs->m_ECS_SceneMap.find(scene)->second.m_sceneIDs.begin();
            if (!ecs::Hierachy::m_GetParent(*entityid)) {
                ecs->m_DeleteEntity(*entityid);
            }
        }


        //remove scene from activescenes
        ecs->m_ECS_SceneMap.erase(scene);

    }

    void SceneManager::m_SaveScene(std::string scene)
    {

        Serialization::Serialize::m_SaveComponentsJson(m_scenePath.find(scene)->second.string());

    }
    void SceneManager::m_SaveAllActiveScenes()
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        for (auto& scenes : ecs->m_ECS_SceneMap) {
            m_SaveScene(scenes.first);
        }
       

    }

    std::optional<std::string> SceneManager::GetSceneByEntityID(ecs::EntityID entityID) {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        for (const auto& [sceneName, entityList] : ecs->m_ECS_SceneMap) {
            // Check if the entityID is in the current vector of entity IDs
            if (std::find(entityList.m_sceneIDs.begin(), entityList.m_sceneIDs.end(), entityID) != entityList.m_sceneIDs.end()) {
                return sceneName;  // Found the matching scene name
            }
        }
        return std::nullopt;  // No match found
    }

    void SceneManager::m_SwapScenes(std::string oldscene, std::string newscene , ecs::EntityID id)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        std::vector<ecs::EntityID>& vectorenityid = ecs->m_ECS_SceneMap.find(oldscene)->second.m_sceneIDs;
        std::vector<ecs::EntityID>::iterator it = std::find(vectorenityid.begin(), vectorenityid.end(), id);
        if (it == vectorenityid.end()) {
            LOGGING_ERROR("Entity not in old scene");
            return;
        }
        
        vectorenityid.erase(it);

        ecs->m_ECS_SceneMap.find(newscene)->second.m_sceneIDs.push_back(id);

        //assign all of entity's scene component into new scene
        for (size_t n{}; n < ecs::TOTALTYPECOMPONENT; n++) {
            if (ecs->m_ECS_EntityMap.find(id)->second.test((ecs::ComponentType)n)) {
                ecs::Component* comp = static_cast<ecs::Component*>(ecs->m_ECS_CombinedComponentPool[(ecs::ComponentType)n]->m_GetEntityComponent(id));
                if (comp) {
                    comp->m_scene = newscene;
                }

            }
        }



    }
}

