/******************************************************************/
/*!
\file      json_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file handles JSON-related operations for component serialization and deserialization in the ECS framework.
			- m_LoadConfig: Loads configuration settings such as window dimensions and FPS cap from a config file.
			- m_JsonFileValidation: Validates if the JSON file exists and creates a new one if it doesn't.
			- m_LoadComponentsJson: Loads component data from a JSON file into the ECS.
			- m_SaveComponentsJson: Saves component data from the ECS to a JSON file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/



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
#include "../ECS/Hierachy.h"
#include <unordered_set>

namespace Serialization {

	void Serialize::m_LoadConfig() {
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
		str >> temp >> help->m_windowHeight;
		//get width
		line.clear();
		temp.clear();
		std::getline(file, line);
		std::stringstream str2{ line };
		str2 >> temp >> help->m_windowWidth;

		line.clear();
		temp.clear();
		std::getline(file, line);
		std::stringstream str3{ line };
		str3 >> temp >> help->m_fpsCap;

		if (help->m_windowHeight <= 0 || help->m_windowWidth <= 0 || !help->m_fpsCap) {
			std::cout << "Error Reading Config file (Width or Height <= 0)" << std::endl;
		}
	}

	void Serialize::m_JsonFileValidation(const std::string& filePath) {

		std::ifstream checkFile(filePath);

		if (!checkFile) {
			std::ofstream createFile(filePath);
			createFile << "[]";  // Initialize empty JSON array
			createFile.close();
		}
		checkFile.close();
	}

	void Serialize::m_LoadComponentsJson(const std::string& jsonFilePath)
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

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();

		// Iterate through each component entry in the JSON array
		for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
			const rapidjson::Value& entityData = doc[i];

			// Create a new entity
			ecs::EntityID newEntityId = ecs->m_CreateEntity();

			// Load the name field
			if (entityData.HasMember("name") && entityData["name"].IsString()) {
				ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityId));
				nc->m_entityName = entityData["name"].GetString();  // Store the name
			}

			// Load Transform Component if it exists
			if (entityData.HasMember("transform") && entityData["transform"].IsObject()) {
				ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(newEntityId));

				const rapidjson::Value& transform = entityData["transform"];
				if (transform.HasMember("position") && transform["position"].IsObject()) {
					tc->m_position.m_x = transform["position"]["x"].GetFloat();
					tc->m_position.m_y = transform["position"]["y"].GetFloat();
				}
				if (transform.HasMember("rotation")) {
					tc->m_rotation = transform["rotation"].GetFloat();
				}
				if (transform.HasMember("scale") && transform["scale"].IsObject()) {
					tc->m_scale.m_x = transform["scale"]["x"].GetFloat();
					tc->m_scale.m_y = transform["scale"]["y"].GetFloat();
				}
			}

			// Load Movement Component if it exists
			

			// Load Collider Component if it exists
			if (entityData.HasMember("collider") && entityData["collider"].IsObject()) {

				ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, newEntityId));

				if (cc) {
					const rapidjson::Value& collider = entityData["collider"];
					if (collider.HasMember("size") && collider["size"].IsObject()) {
						cc->m_Size.m_x = collider["size"]["x"].GetFloat();
						cc->m_Size.m_y = collider["size"]["y"].GetFloat();
					}
					if (collider.HasMember("offset") && collider["offset"].IsObject()) {
						cc->m_OffSet.m_x = collider["offset"]["x"].GetFloat();
						cc->m_OffSet.m_y = collider["offset"]["y"].GetFloat();
					}
					if (collider.HasMember("drawDebug")) {
						cc->m_drawDebug = collider["drawDebug"].GetBool();
					}
				}
			}

			// Load Player Component if it exists
			if (entityData.HasMember("player") && entityData["player"].IsObject()) {
				ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, newEntityId));

				if (pc) {
					const rapidjson::Value& player = entityData["player"];
					if (player.HasMember("control")) {
						pc->m_Control = player["control"].GetBool();
					}
				}
			}

			// Load RigidBody Component if it exists
			if (entityData.HasMember("rigidbody") && entityData["rigidbody"].IsObject()) {
				ecs::RigidBodyComponent* rb = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, newEntityId));

				if (rb) {
					const rapidjson::Value& rigidbody = entityData["rigidbody"];
					if (rigidbody.HasMember("mass")) {
						rb->m_Mass = rigidbody["mass"].GetFloat();
					}
				}
			}

			// Load Sprite Component if it exists
			if (entityData.HasMember("sprite") && entityData["sprite"].IsObject()) {
				ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, newEntityId));

				if (sc) {
					const rapidjson::Value& sprite = entityData["sprite"];
					if (sprite.HasMember("imageID")) {

					}

				}
			}

			// Load Sprite Component if it exists
			if (entityData.HasMember("text") && entityData["text"].IsObject()) {
				ecs::TextComponent* tc = static_cast<ecs::TextComponent*>(ecs->m_AddComponent(ecs::TYPETEXTCOMPONENT, newEntityId));

				if (tc) {
					const rapidjson::Value& text = entityData["text"];
					if (text.HasMember("text")) {
						tc->m_text = text["text"].GetString();
					}
					if (text.HasMember("fontsize")) {
						tc->m_fontSize = text["fontsize"].GetFloat();
					}
					if (text.HasMember("colour")) {
						tc->m_red = text["colour"]["red"].GetFloat();
						tc->m_green = text["colour"]["green"].GetFloat();
						tc->m_blue = text["colour"]["blue"].GetFloat();
					}
				}
			}
			if (entityData.HasMember("animation") && entityData["animation"].IsObject()) {
				ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_AddComponent(ecs::TYPEANIMATIONCOMPONENT, newEntityId));

				if (ac) {
					const rapidjson::Value& animation = entityData["animation"];
					if (animation.HasMember("frameTimer"))
					{
						ac->m_frameTimer = animation["frameTimer"].GetFloat();
					}
					if (animation.HasMember("isAnimating"))
					{
						ac->m_isAnimating = animation["isAnimating"].GetBool();
					}
				}
			}
			if (entityData.HasMember("camera") && entityData["camera"].IsObject()) {
				ecs::CameraComponent* cc = static_cast<ecs::CameraComponent*>(ecs->m_AddComponent(ecs::TYPECAMERACOMPONENT, newEntityId));

				if (cc) {
					const rapidjson::Value& camera = entityData["camera"];
	
				}
			}

			//Check child obj now
			if (entityData.HasMember("children") && entityData["children"].IsArray()) {
				const rapidjson::Value& childrenArray = entityData["children"];

				// Loop through children array
				for (rapidjson::SizeType j = 0; j < childrenArray.Size(); j++) {
					const rapidjson::Value& childData = childrenArray[j];

					ecs::EntityID childEntityId = ecs->m_CreateEntity();

					// Load the child name
					if (childData.HasMember("name") && childData["name"].IsString()) {
						ecs::NameComponent* childNc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(childEntityId));
						childNc->m_entityName = childData["name"].GetString();
					}

					// Load child TransformComponent
					if (childData.HasMember("transform") && childData["transform"].IsObject()) {
						ecs::TransformComponent* childTc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(childEntityId));
						
						if (!childTc) {
							childTc = static_cast<ecs::TransformComponent*>(ecs->m_AddComponent(ecs::TYPETRANSFORMCOMPONENT, childEntityId));
						}

						const rapidjson::Value& transform = childData["transform"];
						if (transform.HasMember("position") && transform["position"].IsObject()) {
							childTc->m_position.m_x = transform["position"]["x"].GetFloat();
							childTc->m_position.m_y = transform["position"]["y"].GetFloat();
						}
						if (transform.HasMember("rotation")) {
							childTc->m_rotation = transform["rotation"].GetFloat();
						}
						if (transform.HasMember("scale") && transform["scale"].IsObject()) {
							childTc->m_scale.m_x = transform["scale"]["x"].GetFloat();
							childTc->m_scale.m_y = transform["scale"]["y"].GetFloat();
						}
					}



					// Load child ColliderComponent
					if (childData.HasMember("collider") && childData["collider"].IsObject()) {
						ecs::ColliderComponent* childCc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(childEntityId));
						
						if (!childCc) {
							childCc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, childEntityId));
						}

						const rapidjson::Value& collider = childData["collider"];
						if (collider.HasMember("size") && collider["size"].IsObject()) {
							childCc->m_Size.m_x = collider["size"]["x"].GetFloat();
							childCc->m_Size.m_y = collider["size"]["y"].GetFloat();
						}
						if (collider.HasMember("offset") && collider["offset"].IsObject()) {
							childCc->m_OffSet.m_x = collider["offset"]["x"].GetFloat();
							childCc->m_OffSet.m_y = collider["offset"]["y"].GetFloat();
						}
						if (collider.HasMember("drawDebug")) {
							childCc->m_drawDebug = collider["drawDebug"].GetBool();
						}
					}

					// Load child RigidbodyComponent
					if (childData.HasMember("rigidbody") && childData["rigidbody"].IsObject()) {
						ecs::RigidBodyComponent* childRc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(childEntityId));
						// If the component doesn't exist, create and attach it
						if (!childRc) {
							childRc = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, childEntityId));
						}

						const rapidjson::Value& rigidbody = childData["rigidbody"];
						if (rigidbody.HasMember("mass")) {
							childRc->m_Mass = rigidbody["mass"].GetFloat();
						}
					}

					if (childData.HasMember("sprite") && childData["sprite"].IsObject()) {
						ecs::SpriteComponent* childSc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESPRITECOMPONENT]->m_GetEntityComponent(childEntityId));

						if (!childSc) {
							childSc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, childEntityId));
						}

						const rapidjson::Value& sprite = childData["sprite"];
						if (sprite.HasMember("imageID")) {
	
						}
					}

					// Load child PlayerComponent
					if (childData.HasMember("player") && childData["player"].IsObject()) {
						ecs::PlayerComponent* childPc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEPLAYERCOMPONENT]->m_GetEntityComponent(childEntityId));
						
						if (!childPc) {
							childPc = static_cast<ecs::PlayerComponent*>(ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, childEntityId));
						}

						const rapidjson::Value& player = childData["player"];
						if (player.HasMember("control")) {
							childPc->m_Control = player["control"].GetBool();
						}
					}

					// Attach the child entity to the parent entity
					ecs::Hierachy::m_SetParent(newEntityId, childEntityId);
				}
			}
		}
		LOGGING_INFO("Load Json Successful");
	}

	void Serialize::m_SaveComponentsJson(const std::string& filePath)
	{
		std::string jsonFilePath = filePath;
		m_JsonFileValidation(jsonFilePath);

		// Create JSON object to hold the updated values
		rapidjson::Document doc;
		doc.SetArray();  // Initialize as an empty array

		rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

		std::unordered_set<ecs::EntityID> savedEntities;  //track saved entities

		auto* ecs = ecs::ECS::m_GetInstance();

		// Iterate through the ECS_Entitymap to save data only for active entities
		for (const auto& entityPair : ecs->m_ECS_EntityMap) {
			ecs::EntityID entityId = entityPair.first;

			if (savedEntities.find(entityId) != savedEntities.end()) {
				continue;
			}
			if (!ecs::Hierachy::m_GetParent(entityId).has_value()) {

				// Create a new JSON object for entitydata
				rapidjson::Value entityData(rapidjson::kObjectType);

				bool hasComponents = false;

				// Find name for this entity using objEntityId
				if (entityPair.second.test(ecs::ComponentType::TYPENAMECOMPONENT)) {
					rapidjson::Value nameValue;
					nameValue.SetString(static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(entityId))->m_entityName.c_str(), allocator);
					entityData.AddMember("name", nameValue, allocator);
					hasComponents = true;
				}

				// Check if the entity has TransformComponent and save 
				if (entityPair.second.test(ecs::ComponentType::TYPETRANSFORMCOMPONENT)) {
					ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(entityId));
					if (tc) {
						rapidjson::Value transform(rapidjson::kObjectType);
						transform.AddMember("position", rapidjson::Value().SetObject()
							.AddMember("x", tc->m_position.m_x, allocator)
							.AddMember("y", tc->m_position.m_y, allocator), allocator);
						transform.AddMember("rotation", tc->m_rotation, allocator);
						transform.AddMember("scale", rapidjson::Value().SetObject()
							.AddMember("x", tc->m_scale.m_x, allocator)
							.AddMember("y", tc->m_scale.m_y, allocator), allocator);
						entityData.AddMember("transform", transform, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}



				// Check if the entity has ColliderComponent and save it
				if (entityPair.second.test(ecs::ComponentType::TYPECOLLIDERCOMPONENT)) {
					ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(entityId));
					if (cc) {
						rapidjson::Value collider(rapidjson::kObjectType);
						collider.AddMember("size", rapidjson::Value().SetObject()
							.AddMember("x", cc->m_Size.m_x, allocator)
							.AddMember("y", cc->m_Size.m_y, allocator), allocator);
						collider.AddMember("offset", rapidjson::Value().SetObject()
							.AddMember("x", cc->m_OffSet.m_x, allocator)
							.AddMember("y", cc->m_OffSet.m_y, allocator), allocator);
						collider.AddMember("drawDebug", cc->m_drawDebug, allocator);
						entityData.AddMember("collider", collider, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				// Check if the entity has PlayerComponent and save it
				if (entityPair.second.test(ecs::ComponentType::TYPEPLAYERCOMPONENT)) {
					ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEPLAYERCOMPONENT]->m_GetEntityComponent(entityId));
					if (pc) {
						rapidjson::Value player(rapidjson::kObjectType);
						player.AddMember("control", pc->m_Control, allocator);
						entityData.AddMember("player", player, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				// Check if the entity has RigidBodyComponent and save it
				if (entityPair.second.test(ecs::ComponentType::TYPERIGIDBODYCOMPONENT)) {
					ecs::RigidBodyComponent* rb = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(entityId));
					if (rb) {
						rapidjson::Value rigidbody(rapidjson::kObjectType);
						rigidbody.AddMember("mass", rb->m_Mass, allocator);
						entityData.AddMember("rigidbody", rigidbody, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				// Check if the entity has SpriteComponent and save it
				if (entityPair.second.test(ecs::ComponentType::TYPESPRITECOMPONENT)) {
					ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPESPRITECOMPONENT]->m_GetEntityComponent(entityId));
					if (sc) {
						rapidjson::Value sprite(rapidjson::kObjectType);

						entityData.AddMember("sprite", sprite, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				if (entityPair.second.test(ecs::ComponentType::TYPETEXTCOMPONENT)) {
					ecs::TextComponent* tc = static_cast<ecs::TextComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPETEXTCOMPONENT]->m_GetEntityComponent(entityId));
					if (tc) {
						rapidjson::Value text(rapidjson::kObjectType);
						rapidjson::Value textValue;
						textValue.SetString(tc->m_text.c_str(), allocator);

						text.AddMember("text", textValue, allocator);
						text.AddMember("fontsize", tc->m_fontSize, allocator);
						text.AddMember("colour", rapidjson::Value().SetObject()
							.AddMember("red", tc->m_red, allocator)
							.AddMember("blue", tc->m_blue, allocator)
							.AddMember("green", tc->m_green, allocator), allocator);

						entityData.AddMember("text", text, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				if (entityPair.second.test(ecs::ComponentType::TYPEANIMATIONCOMPONENT)) {
					ecs::AnimationComponent* ac = static_cast<ecs::AnimationComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(entityId));
					if (ac) {
						rapidjson::Value animation(rapidjson::kObjectType);

						animation.AddMember("framesPerSecond", ac->m_framesPerSecond, allocator);
						animation.AddMember("frameTimer", ac->m_frameTimer, allocator);
						animation.AddMember("isAnimating", ac->m_isAnimating, allocator);
						entityData.AddMember("animation", animation, allocator);
						hasComponents = true;  // Mark as having a component
					}
				}

				if (entityPair.second.test(ecs::ComponentType::TYPECAMERACOMPONENT))
				{
					ecs::CameraComponent* cc = static_cast<ecs::CameraComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPECAMERACOMPONENT]->m_GetEntityComponent(entityId));
					if (cc)
					{
						rapidjson::Value camera(rapidjson::kObjectType);
						/*camera.AddMember("planes", rapidjson::Value().SetObject()
							.AddMember("left", cc->m_left, allocator)
							.AddMember("right", cc->m_right, allocator)
							.AddMember("top", cc->m_top, allocator)
							.AddMember("bottom", cc->m_bottom, allocator), allocator);
						camera.AddMember("aspectRatio", cc->m_aspectRatio, allocator);*/
					
						entityData.AddMember("camera", camera, allocator);
						hasComponents = true;
					}
				}

				

				//Check Child Obj now
				std::optional<std::vector<ecs::EntityID>> childrenOptional = ecs::Hierachy::m_GetChild(entityId);
				if (childrenOptional.has_value()) {
					std::vector<ecs::EntityID> children = childrenOptional.value();
					if (!children.empty()) {
						rapidjson::Value childrenArray(rapidjson::kArrayType);
						for (const auto& childID : children) {
							rapidjson::Value childData(rapidjson::kObjectType);

							// Save the child name
							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPENAMECOMPONENT)) {
								rapidjson::Value childNameValue;
								childNameValue.SetString(static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(childID))->m_entityName.c_str(), allocator);
								childData.AddMember("name", childNameValue, allocator);
							}

							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPETRANSFORMCOMPONENT)) {
								ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(childID));
								if (tc) {
									rapidjson::Value transform(rapidjson::kObjectType);
									transform.AddMember("position", rapidjson::Value().SetObject()
										.AddMember("x", tc->m_position.m_x, allocator)
										.AddMember("y", tc->m_position.m_y, allocator), allocator);
									transform.AddMember("rotation", tc->m_rotation, allocator);
									transform.AddMember("scale", rapidjson::Value().SetObject()
										.AddMember("x", tc->m_scale.m_x, allocator)
										.AddMember("y", tc->m_scale.m_y, allocator), allocator);
									childData.AddMember("transform", transform, allocator);
								}
							}

							// Save child MovementComponent


							// Save child ColliderComponent
							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPECOLLIDERCOMPONENT)) {
								ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(childID));
								if (cc) {
									rapidjson::Value collider(rapidjson::kObjectType);
									collider.AddMember("size", rapidjson::Value().SetObject()
										.AddMember("x", cc->m_Size.m_x, allocator)
										.AddMember("y", cc->m_Size.m_y, allocator), allocator);
									collider.AddMember("offset", rapidjson::Value().SetObject()
										.AddMember("x", cc->m_OffSet.m_x, allocator)
										.AddMember("y", cc->m_OffSet.m_y, allocator), allocator);
									collider.AddMember("drawDebug", cc->m_drawDebug, allocator);
									childData.AddMember("collider", collider, allocator);
								}
							}

							// Save child RigidbodyComponent
							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPERIGIDBODYCOMPONENT)) {
								ecs::RigidBodyComponent* rc = static_cast<ecs::RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(childID));
								if (rc) {
									rapidjson::Value rigidbody(rapidjson::kObjectType);
									rigidbody.AddMember("mass", rc->m_Mass, allocator);
									childData.AddMember("rigidbody", rigidbody, allocator);
								}
							}

							// Save child SpriteComponent
							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPESPRITECOMPONENT)) {
								ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPESPRITECOMPONENT]->m_GetEntityComponent(childID));
								if (sc) {
									rapidjson::Value sprite(rapidjson::kObjectType);

									childData.AddMember("sprite", sprite, allocator);
								}
							}

							// Save child PlayerComponent
							if (ecs->m_ECS_EntityMap[childID].test(ecs::ComponentType::TYPEPLAYERCOMPONENT)) {
								ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::ComponentType::TYPEPLAYERCOMPONENT]->m_GetEntityComponent(childID));
								if (pc) {
									rapidjson::Value player(rapidjson::kObjectType);
									player.AddMember("control", pc->m_Control, allocator);
									childData.AddMember("player", player, allocator);
								}
							}


							// Add this child to the parent's children array
							childrenArray.PushBack(childData, allocator);
						}
						entityData.AddMember("children", childrenArray, allocator); 
					}
				}

				if (hasComponents) {
					doc.PushBack(entityData, allocator);
				}
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
