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
#include "../Debugging/Logging.h"
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
    void Serialize::JsonFileValidation(const std::string& filePath) {

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
                    tc->position.m_x = transform["position"]["x"].GetFloat();
                    tc->position.m_y = transform["position"]["y"].GetFloat();
                }
                if (transform.HasMember("rotation")) {
                    tc->rotation = transform["rotation"].GetFloat();
                }
                if (transform.HasMember("scale") && transform["scale"].IsObject()) {
                    tc->scale.m_x = transform["scale"]["x"].GetFloat();
                    tc->scale.m_y = transform["scale"]["y"].GetFloat();
                }

                // Mark the entity as having a TransformComponent in the ECS_EntityMap
                ecs->ECS_EntityMap[newEntityID].set(Ecs::TypeTransformComponent);
            }

            // Load Movement Component if it exists
            if (entityData.HasMember("movement") && entityData["movement"].IsObject()) {

                ecs->AddComponent(Ecs::TypeMovemmentComponent, newEntityID);

                Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeMovemmentComponent]->GetEntityComponent(newEntityID));

                const rapidjson::Value& movement = entityData["movement"];

                if (movement.HasMember("speed")) {
                    mc->Speed = movement["speed"].GetFloat();
                }
                if (movement.HasMember("direction") && movement["direction"].IsObject()) {
                    mc->Direction.m_x = movement["direction"]["x"].GetFloat();
                    mc->Direction.m_y = movement["direction"]["y"].GetFloat();
                }

                // Mark the entity as having a MovementComponent in the ECS_EntityMap
                ecs->ECS_EntityMap[newEntityID].set(Ecs::TypeMovemmentComponent);
            }

            // Load Collider Component if it exists
            if (entityData.HasMember("collider") && entityData["collider"].IsObject()) {

                ecs->AddComponent(Ecs::TypeColliderComponent, newEntityID);

                Ecs::ColliderComponent* cc = static_cast<Ecs::ColliderComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeColliderComponent]->GetEntityComponent(newEntityID));

                if (cc) {
                    const rapidjson::Value& collider = entityData["collider"];
                    if (collider.HasMember("size") && collider["size"].IsObject()) {
                        cc->Size.m_x = collider["size"]["x"].GetFloat();
                        cc->Size.m_y = collider["size"]["y"].GetFloat();
                    }
                    if (collider.HasMember("offset") && collider["offset"].IsObject()) {
                        cc->OffSet.m_x = collider["offset"]["x"].GetFloat();
                        cc->OffSet.m_y = collider["offset"]["y"].GetFloat();
                    }
                    if (collider.HasMember("layer")) {
                        cc->Layer = collider["layer"].GetUint();
                    }
                    if (collider.HasMember("drawDebug")) {
                        cc->drawDebug = collider["drawDebug"].GetBool();
                    }
                }
            }

            // Load Player Component if it exists
            if (entityData.HasMember("player") && entityData["player"].IsObject()) {
                ecs->AddComponent(Ecs::TypePlayerComponent, newEntityID);
                Ecs::PlayerComponent* pc = static_cast<Ecs::PlayerComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypePlayerComponent]->GetEntityComponent(newEntityID));

                if (pc) {
                    const rapidjson::Value& player = entityData["player"];
                    if (player.HasMember("control")) {
                        pc->Control = player["control"].GetBool();
                    }
                }
            }

            // Load RigidBody Component if it exists
            if (entityData.HasMember("rigidbody") && entityData["rigidbody"].IsObject()) {
                ecs->AddComponent(Ecs::TypeRigidBodyComponent, newEntityID);
                Ecs::RigidBodyComponent* rb = static_cast<Ecs::RigidBodyComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeRigidBodyComponent]->GetEntityComponent(newEntityID));

                if (rb) {
                    const rapidjson::Value& rigidbody = entityData["rigidbody"];
                    if (rigidbody.HasMember("mass")) {
                        rb->Mass = rigidbody["mass"].GetFloat();
                    }
                }
            }

            // Load Sprite Component if it exists
            if (entityData.HasMember("sprite") && entityData["sprite"].IsObject()) {
                ecs->AddComponent(Ecs::TypeSpriteComponent, newEntityID);
                Ecs::SpriteComponent* sc = static_cast<Ecs::SpriteComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeSpriteComponent]->GetEntityComponent(newEntityID));

                if (sc) {
                    const rapidjson::Value& sprite = entityData["sprite"];
                    if (sprite.HasMember("imageID")) {
                        sc->imageID = sprite["imageID"].GetUint();
                    }
                    if (sprite.HasMember("frameNumber")) {
                        sc->frameNumber = sprite["frameNumber"].GetUint();
                    }
                }
            }
        }


        LOGGING_INFO("Load Json Successful");
    }

    void Serialize::SaveComponentsJson(const std::string& filePath, const std::unordered_map<Ecs::EntityID, std::bitset<Ecs::ComponentType::TotalTypeComponent>>& ECS_EntityMap, const std::vector<std::string>& obj_text_entries, const std::vector<Ecs::EntityID>& obj_entity_id)
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

            bool hasComponents = false;


            // Find the corresponding name for this entity using obj_entity_id
            auto it = std::find(obj_entity_id.begin(), obj_entity_id.end(), entityID);
            if (it != obj_entity_id.end()) {
                size_t index = std::distance(obj_entity_id.begin(), it);
                if (index < obj_text_entries.size()) {
                    rapidjson::Value nameValue;
                    nameValue.SetString(obj_text_entries[index].c_str(), allocator);
                    entityData.AddMember("name", nameValue, allocator);
                    hasComponents = true;
                }
            }

            // Check if the entity has TransformComponent and save 
            if (entityPair.second.test(Ecs::ComponentType::TypeTransformComponent)) {
                Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeTransformComponent]->GetEntityComponent(entityID));
                if (tc) {
                    rapidjson::Value transform(rapidjson::kObjectType);
                    transform.AddMember("position", rapidjson::Value().SetObject()
                        .AddMember("x", tc->position.m_x, allocator)
                        .AddMember("y", tc->position.m_y, allocator), allocator);
                    transform.AddMember("rotation", tc->rotation, allocator);
                    transform.AddMember("scale", rapidjson::Value().SetObject()
                        .AddMember("x", tc->scale.m_x, allocator)
                        .AddMember("y", tc->scale.m_y, allocator), allocator);
                    entityData.AddMember("transform", transform, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            // Check if the entity has MovementComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypeMovemmentComponent)) {
                Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeMovemmentComponent]->GetEntityComponent(entityID));
                if (mc) {
                    rapidjson::Value movement(rapidjson::kObjectType);
                    movement.AddMember("speed", mc->Speed, allocator);
                    movement.AddMember("direction", rapidjson::Value().SetObject()
                        .AddMember("x", mc->Direction.m_x, allocator)
                        .AddMember("y", mc->Direction.m_y, allocator), allocator);
                    entityData.AddMember("movement", movement, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            // Check if the entity has ColliderComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypeColliderComponent)) {
                Ecs::ColliderComponent* cc = static_cast<Ecs::ColliderComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeColliderComponent]->GetEntityComponent(entityID));
                if (cc) {
                    rapidjson::Value collider(rapidjson::kObjectType);
                    collider.AddMember("size", rapidjson::Value().SetObject()
                        .AddMember("x", cc->Size.m_x, allocator)
                        .AddMember("y", cc->Size.m_y, allocator), allocator);
                    collider.AddMember("offset", rapidjson::Value().SetObject()
                        .AddMember("x", cc->OffSet.m_x, allocator)
                        .AddMember("y", cc->OffSet.m_y, allocator), allocator);
                    collider.AddMember("layer", cc->Layer, allocator);
                    collider.AddMember("drawDebug", cc->drawDebug, allocator);
                    entityData.AddMember("collider", collider, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            // Check if the entity has PlayerComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypePlayerComponent)) {
                Ecs::PlayerComponent* pc = static_cast<Ecs::PlayerComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypePlayerComponent]->GetEntityComponent(entityID));
                if (pc) {
                    rapidjson::Value player(rapidjson::kObjectType);
                    player.AddMember("control", pc->Control, allocator);
                    entityData.AddMember("player", player, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            // Check if the entity has RigidBodyComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypeRigidBodyComponent)) {
                Ecs::RigidBodyComponent* rb = static_cast<Ecs::RigidBodyComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeRigidBodyComponent]->GetEntityComponent(entityID));
                if (rb) {
                    rapidjson::Value rigidbody(rapidjson::kObjectType);
                    rigidbody.AddMember("mass", rb->Mass, allocator);
                    entityData.AddMember("rigidbody", rigidbody, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            // Check if the entity has SpriteComponent and save it
            if (entityPair.second.test(Ecs::ComponentType::TypeSpriteComponent)) {
                Ecs::SpriteComponent* sc = static_cast<Ecs::SpriteComponent*>(ecs->ECS_CombinedComponentPool[Ecs::ComponentType::TypeSpriteComponent]->GetEntityComponent(entityID));
                if (sc) {
                    rapidjson::Value sprite(rapidjson::kObjectType);
                    sprite.AddMember("imageID", sc->imageID, allocator);
                    sprite.AddMember("frameNumber", sc->frameNumber, allocator);
                    entityData.AddMember("sprite", sprite, allocator);
                    hasComponents = true;  // Mark as having a component
                }
            }

            if (hasComponents) {
                doc.PushBack(entityData, allocator);
            }
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

        LOGGING_INFO("Save Json Successful");
    }
}
