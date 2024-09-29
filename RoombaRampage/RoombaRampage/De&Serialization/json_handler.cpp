#define _CRT_SECURE_NO_WARNINGS

#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/filewritestream.h"
#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/istreamwrapper.h"
#include "../Dependencies/rapidjson/prettywriter.h"
#include "../Dependencies/rapidjson/filereadstream.h"

#include "../ECS/ECS.h"
#include "../Application/Helper.h"
#include "json_handler.h"

#include <cstdio>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

namespace Serialization {

    void Serialize::LoadConfig() {
        std::ifstream file;
        file.open("./Config/Config.txt");

        if (!file.is_open()) {
            std::cerr << "Error opening config file" << std::endl;
            return;
        }
        Helper::Helpers* help = Helper::Helpers::GetInstance();

        std::string line;
        std::string temp;

        //get height
        std::getline(file, line);
        std::stringstream str{ line };
        str >> temp >> help->WindowHeight;
        //get width
        line.clear();
        temp.clear();
        std::getline(file, line);
        std::stringstream str2{ line };
        str2 >> temp >> help->WindowWidth;

        line.clear();
        temp.clear();
        std::getline(file,line);
        std::stringstream str3{ line };
        str3 >> temp >> help->FpsCap;

        if (help->WindowHeight <= 0 || help->WindowWidth <= 0 || !help->FpsCap) {
            std::cout << "Error Reading Config file (Width or Height <= 0)" << std::endl;
        }
    }
    // Helper function to check if the json file exists and create it
    void JsonFileValidation(const std::string& filePath) {

        std::ifstream checkFile(filePath);

        if (!checkFile) {
            std::ofstream createFile(filePath);
            createFile << "[]";  // Initialize empty JSON array
            createFile.close();
        }
        checkFile.close();
    }

    void Serialize::LoadComponentsJson(const std::string& jsonFilePath, Ecs::ECS* ecs, std::vector<std::string>& obj_text_entries)
    {
        // Open the JSON file for reading
        std::ifstream inputFile(jsonFilePath);

        if (!inputFile) {
            std::cerr << "Failed to open JSON file for reading: " << jsonFilePath << std::endl;
            return;
        }

        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        // Parse the JSON content
        rapidjson::Document doc;
        doc.Parse(fileContent.c_str());

        obj_text_entries.clear();

        // Iterate through each component entry in the JSON array
        for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
            const rapidjson::Value& entityData = doc[i];

            // Create a new entity
            Ecs::EntityID newEntityID = ecs->CreateEntity();

            // Load the name field
            if (entityData.HasMember("name") && entityData["name"].IsString()) {
                obj_text_entries.push_back(entityData["name"].GetString());  // Store the name
            }
            else {
                obj_text_entries.push_back("GameObject -"); //Incase cannot load
            }

            // Load Transform Component if it exists
            if (entityData.HasMember("transform") && entityData["transform"].IsObject()) {
                Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeTransformComponent]->GetEntityComponent(newEntityID));

                const rapidjson::Value& transform = entityData["transform"];
                if (transform.HasMember("position") && transform["position"].IsObject()) {
                    tc->position.x = transform["position"]["x"].GetFloat();
                    tc->position.y = transform["position"]["y"].GetFloat();
                }
                if (transform.HasMember("rotation")) {
                    tc->rotation = transform["rotation"].GetFloat();
                }
                if (transform.HasMember("scale") && transform["scale"].IsObject()) {
                    tc->scale.x = transform["scale"]["x"].GetFloat();
                    tc->scale.y = transform["scale"]["y"].GetFloat();
                }

                // Mark the entity as having a TransformComponent in the ECS_EntityMap
                ecs->ECS_EntityMap[newEntityID].set(Ecs::TypeTransformComponent);
            }

            // Load Movement Component if it exists
            if (entityData.HasMember("movement") && entityData["movement"].IsObject()) {
                Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeMovemmentComponent]->GetEntityComponent(newEntityID));

                const rapidjson::Value& movement = entityData["movement"];
                if (movement.HasMember("speed")) {
                    mc->Speed = movement["speed"].GetFloat();
                }
                if (movement.HasMember("direction") && movement["direction"].IsObject()) {
                    mc->Direction.x = movement["direction"]["x"].GetFloat();
                    mc->Direction.y = movement["direction"]["y"].GetFloat();
                }

                // Mark the entity as having a MovementComponent in the ECS_EntityMap
                ecs->ECS_EntityMap[newEntityID].set(Ecs::TypeMovemmentComponent);
            }
        }
    }

    void Serialize::SaveComponentsJson(const std::string& filePath, const std::unordered_map<Ecs::EntityID, std::bitset<Ecs::ComponentType::TotalTypeComponent>>& ECS_EntityMap, const std::vector<std::string>& obj_text_entries)
    {
        // JSON File Validation / Creation
        std::string jsonFilePath = filePath + "/components.json";
        JsonFileValidation(jsonFilePath);

        // Create JSON object to hold the updated values
        rapidjson::Document doc;
        doc.SetArray();  // Initialize as an empty array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        auto* ecs = Ecs::ECS::GetInstance();

        size_t entityIndex = 0;

        // Iterate through the ECS_Entitymap to save data only for active entities
        for (const auto& entityPair : ECS_EntityMap) {
            Ecs::EntityID entityID = entityPair.first;

            // Create a new JSON object for entitydata
            rapidjson::Value entityData(rapidjson::kObjectType);

            // Add the name of the entity
            if (entityIndex < obj_text_entries.size()) {
                rapidjson::Value nameValue;
                nameValue.SetString(obj_text_entries[entityIndex].c_str(), allocator);  // Set the name field
                entityData.AddMember("name", nameValue, allocator);
            }

            // Check if the entity has TransformComponent and save 
            if (entityPair.second.test(Ecs::ComponentType::TypeTransformComponent)) {
                Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeTransformComponent]->GetEntityComponent(entityID));
                if (tc) {
                    rapidjson::Value transform(rapidjson::kObjectType);
                    transform.AddMember("position", rapidjson::Value().SetObject()
                        .AddMember("x", tc->position.x, allocator)
                        .AddMember("y", tc->position.y, allocator), allocator);
                    transform.AddMember("rotation", tc->rotation, allocator);
                    transform.AddMember("scale", rapidjson::Value().SetObject()
                        .AddMember("x", tc->scale.x, allocator)
                        .AddMember("y", tc->scale.y, allocator), allocator);
                    entityData.AddMember("transform", transform, allocator);
                }
            }

            // Check if the entity has MovementComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypeMovemmentComponent)) {
                Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeMovemmentComponent]->GetEntityComponent(entityID));
                if (mc) {
                    rapidjson::Value movement(rapidjson::kObjectType);
                    movement.AddMember("speed", mc->Speed, allocator);
                    movement.AddMember("direction", rapidjson::Value().SetObject()
                        .AddMember("x", mc->Direction.x, allocator)
                        .AddMember("y", mc->Direction.y, allocator), allocator);
                    entityData.AddMember("movement", movement, allocator);
                }
            }

            doc.PushBack(entityData, allocator);
            ++entityIndex;
        }

        // Write the JSON back to file
        rapidjson::StringBuffer writeBuffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
        doc.Accept(writer);

        std::ofstream outputFile(jsonFilePath);
        if (outputFile) {
            outputFile << writeBuffer.GetString();
            outputFile.close();
        }
    }
}
