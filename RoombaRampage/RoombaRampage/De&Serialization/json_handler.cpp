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
#include "json_handler.h"

#include <cstdio>
#include <memory>
#include <fstream>
#include <iostream>

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

void LoadComponentsJson(const std::string& jsonFilePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc)
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

void SaveComponentsJson(const std::string& filePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc, int entityIndex)
{

	//checks to see if entity is alive
	Ecs::ECS *ecs = Ecs::ECS::GetInstance();

	bool save = false;

	for (auto& Entiy : ecs->ECS_EntityMap) {
		if (Entiy.first == entityIndex) {
			save = true;
			break;
		}
	}
	




	//JSON File Validation / Creation
	std::string jsonFilePath = filePath + "/components.json";
	JsonFileValidation(jsonFilePath);

	//Create json obj to hold the updated values
	std::ifstream inputFile(jsonFilePath);
	//clear json file
	rapidjson::Document doc;

	doc.SetArray();  // Set an empty array

	if (inputFile){
		std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());  // Read entire file into a string

		// Parse the existing JSON data from the string
		doc.Parse(fileContent.c_str());
		inputFile.close();
	}

	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	//rapidjson::Value entity(rapidjson::kObjectType);

	if (entityIndex < 0 || entityIndex >= doc.Size()) {

		rapidjson::Value entity(rapidjson::kObjectType);

		if (tc != nullptr)
		{
			// Create transform component in JSON
			rapidjson::Value transform(rapidjson::kObjectType);
			transform.AddMember("position", rapidjson::Value().SetObject()
				.AddMember("x", tc->position.x, allocator)
				.AddMember("y", tc->position.y, allocator), allocator);
			transform.AddMember("rotation", tc->rotation, allocator);
			transform.AddMember("scale", rapidjson::Value().SetObject()
				.AddMember("x", tc->scale.x, allocator)
				.AddMember("y", tc->scale.y, allocator), allocator);

			entity.AddMember("transform", transform, allocator);
		}

		if (mc != nullptr)
		{
			// Create movement component in JSON
			rapidjson::Value movement(rapidjson::kObjectType);
			movement.AddMember("speed", mc->Speed, allocator);
			movement.AddMember("direction", rapidjson::Value().SetObject()
				.AddMember("x", mc->Direction.x, allocator)
				.AddMember("y", mc->Direction.y, allocator), allocator);
			entity.AddMember("movement", movement, allocator);
		}
		doc.PushBack(entity, allocator);
	}
	else
	{
		// Modify the existing game object
		rapidjson::Value& gameObject = doc[entityIndex];

		// Modify transform component
		if (tc != nullptr && gameObject.HasMember("transform")) {
			rapidjson::Value& transform = gameObject["transform"];
			transform["position"]["x"].SetFloat(tc->position.x);
			transform["position"]["y"].SetFloat(tc->position.y);
			transform["rotation"].SetFloat(tc->rotation);
			transform["scale"]["x"].SetFloat(tc->scale.x);
			transform["scale"]["y"].SetFloat(tc->scale.y);
		}

		// Modify movement component
		if (mc != nullptr && gameObject.HasMember("movement")) {
			rapidjson::Value& movement = gameObject["movement"];
			movement["speed"].SetFloat(mc->Speed);
			movement["direction"]["x"].SetFloat(mc->Direction.x);
			movement["direction"]["y"].SetFloat(mc->Direction.y);
		}
	}

	rapidjson::StringBuffer writeBuffer;
	rapidjson::PrettyWriter <rapidjson::StringBuffer>  writer(writeBuffer);

	// Write the JSON
	doc.Accept(writer);

	std::ofstream outputFile(jsonFilePath);

	if (outputFile)
	{
		std::ofstream outputFile(jsonFilePath);
		outputFile << writeBuffer.GetString();
		outputFile.close();
	}
}