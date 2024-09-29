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

    void Serialize::LoadComponentsJson(const std::string& jsonFilePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc)
    {
        //FILE* jsonFile = fopen(jsonFilePath.c_str(), "wb"); //Open json file
        ////Read json file into buffer
        //std::unique_ptr<char[]> writeBuffer(new char[65536]);

        ////rapidjson::StringBuffer writeBuffer;
        ////Write the JSON data to the file
        //rapidjson::FileWriteStream os(jsonFile, writeBuffer.get(), 65536);
        //rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

        //Create json obj to hold the updated values
        rapidjson::Document doc;
        doc.SetObject();

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator(); //Alocate memory for dynamic changes to json files

        // Create transform component in JSON
        rapidjson::Value entity(rapidjson::kObjectType);
        rapidjson::Value transform(rapidjson::kObjectType);
        transform.AddMember("position", rapidjson::Value().SetObject()
            .AddMember("x", tc->position.x, allocator)
            .AddMember("y", tc->position.y, allocator), allocator);
        transform.AddMember("rotation", tc->rotation, allocator);
        transform.AddMember("scale", rapidjson::Value().SetObject()
            .AddMember("x", tc->scale.x, allocator)
            .AddMember("y", tc->scale.y, allocator), allocator);

        entity.AddMember("transform", transform, allocator);

        // Create movement component in JSON
        rapidjson::Value movement(rapidjson::kObjectType);
        movement.AddMember("speed", mc->Speed, allocator);
        movement.AddMember("direction", rapidjson::Value().SetObject()
            .AddMember("x", mc->Direction.x, allocator)
            .AddMember("y", mc->Direction.y, allocator), allocator);

        entity.AddMember("movement", movement, allocator);

        doc.AddMember("entity", entity, allocator);


        rapidjson::StringBuffer writeBuffer;
        rapidjson::PrettyWriter <rapidjson::StringBuffer>  writer(writeBuffer);

        // Write the JSON
        doc.Accept(writer);

        FILE* jsonFile = fopen(jsonFilePath.c_str(), "wb");  // Open JSON file for writing

        if (jsonFile)
        {
            fputs(writeBuffer.GetString(), jsonFile);
            fclose(jsonFile);
        }
    }

    void Serialize::SaveComponentsJson(const std::string& filePath, const std::unordered_map<Ecs::EntityID, std::bitset<Ecs::ComponentType::TotalTypeComponent>>& ECS_EntityMap)
    {
        // JSON File Validation / Creation
        std::string jsonFilePath = filePath + "/components.json";
        JsonFileValidation(jsonFilePath);

        // Create JSON object to hold the updated values
        rapidjson::Document doc;
        doc.SetArray();  // Initialize as an empty array

        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        auto* ecs = Ecs::ECS::GetInstance();

        // Iterate through the ECS_Entitymap to save data only for active entities
        for (const auto& entityPair : ECS_EntityMap) {
            Ecs::EntityID entityID = entityPair.first;

            // Create a new JSON object for entitydata
            rapidjson::Value entityData(rapidjson::kObjectType);

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
