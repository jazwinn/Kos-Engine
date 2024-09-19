#define _CRT_SECURE_NO_WARNINGS

#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/filewritestream.h"

#include "../ECS/ECS.h"
#include "json_handler.h"

#include <cstdio>
#include <memory>

void LoadComponentsJson(const std::string& jsonFilePath, TransformComponent* tc, MovementComponent* mc)
{
	FILE* jsonFile = fopen(jsonFilePath.c_str(), "wb"); //Open json file

	//Read json file into buffer
	std::unique_ptr<char[]> writeBuffer(new char[65536]);
	
	//Write the JSON data to the file
	rapidjson::FileWriteStream os(jsonFile, writeBuffer.get(), 65536);
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

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

	// Write the JSON document
	doc.Accept(writer);
	fclose(jsonFile);
}

void SaveComponentsJson(const std::string& jsonFilePath, TransformComponent* tc, MovementComponent* mc)
{

	FILE* jsonFile = fopen(jsonFilePath.c_str(), "wb");  // Open JSON file for writing

	std::unique_ptr<char[]> writeBuffer(new char[65536]);

	rapidjson::FileWriteStream os(jsonFile, writeBuffer.get(), sizeof(writeBuffer));

	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

	//Create json obj to hold the updated values
	rapidjson::Document doc;
	doc.SetObject();
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

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

	// Write the JSON
	doc.Accept(writer);
	fclose(jsonFile);
}
