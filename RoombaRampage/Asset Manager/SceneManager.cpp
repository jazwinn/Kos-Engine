#include "SceneManager.h"
#include <fstream>
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "filewritestream.h"
#include "../ECS/ECS.h"

namespace scenes {

    std::unique_ptr<SceneManager> SceneManager::m_InstancePtr = nullptr;

    std::string SceneManager::m_AddScene(std::string filepath)
    {

        size_t lastSlash = filepath.find_last_of("/\\");
        std::string fileName = filepath.substr(lastSlash + 1);  // Extract "test.json"

        
        size_t lastDot = fileName.find_last_of('.');
        std::string fileBaseName = fileName.substr(0, lastDot);  // Extract "test"

        if (std::find(m_availableScenes.begin(), m_availableScenes.end(), fileBaseName) != m_availableScenes.end()) {
            //LOGGING_WARN("Scene Already added");
            return fileBaseName;
        }

        m_availableScenes.push_back(fileBaseName);

        return fileBaseName;
    }

    std::optional<std::string> SceneManager::m_CreateNewScene(std::string scene)
    {
        std::string jsonFilePath = m_jsonFilePath + scene + ".json";
        std::ifstream checkFile(jsonFilePath);
        //check if file name exist
        if (checkFile) {
            //if file name exist
            LOGGING_WARN("JSON file already exist, select another name");
            return {};
        }
        //create a json file

        FILE* fp = std::fopen(jsonFilePath.c_str(), "wb");
        // start with []
        fprintf(fp, "[]");
        char writeBuffer[1];  // Buffer to optimize file writing
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

        std::fclose(fp);

        // return file path
        return jsonFilePath;
    }

    void SceneManager::m_LoadScene(std::string scene)
    {
        if (std::find(m_availableScenes.begin(), m_availableScenes.end(), scene) == m_availableScenes.end()) {
            LOGGING_WARN("Scene Not added");
            return;
        }

        if (scene == m_activeScene) {
            LOGGING_WARN("Scene already active");
            return;
        }

        //clear scene
        m_ClearScene(scene);

        // Ensure the JSON file exists

        std::string jsonFilePath = m_jsonFilePath + scene + ".json";
        std::ifstream checkFile(jsonFilePath);
        if (!checkFile) {
            std::cerr << "Error: JSON file not found: " << jsonFilePath << std::endl;
            return;
        }

        // Load entities from the JSON file
        std::cout << "Loading entities from: " << jsonFilePath << std::endl;
        Serialization::Serialize::m_LoadComponentsJson(jsonFilePath);  // Load into ECS

        LOGGING_INFO("Entities successfully loaded!");


        m_activeScene = scene;
    }
    void SceneManager::m_ClearScene(std::string scene)
    {
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        
        std::vector<ecs::EntityID> activeEntity;

        for (auto& entity : ecs->m_ECS_EntityMap) {
            activeEntity.push_back(entity.first);
        }

        //delete all entity
        for (auto& id : activeEntity) {
            ecs->m_DeleteEntity(id);
        }


    }

    void SceneManager::m_SaveScene(std::string scene)
    {
        if (std::find(m_availableScenes.begin(), m_availableScenes.end(), scene) == m_availableScenes.end()) {
            LOGGING_WARN("No scene available to save");
            return;
        }
        std::string jsonFilePath = m_jsonFilePath + scene + ".json";

        Serialization::Serialize::m_SaveComponentsJson(jsonFilePath);

    }
    void SceneManager::m_SaveActiveScene()
    {

        m_SaveScene(m_activeScene);

    }
}

