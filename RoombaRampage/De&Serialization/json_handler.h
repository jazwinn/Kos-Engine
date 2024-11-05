	/********************************************************************/
/*!
\file      json_handler.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 11, 2024
\brief     This header file declares functions that handle JSON-related operations for component serialization and
		   deserialization in the ECS framework.
		   - m_LoadConfig: Loads configuration settings such as window dimensions and FPS cap from a config file.
		   - m_JsonFileValidation: Validates if the JSON file exists and creates a new one if it doesn't.
		   - m_LoadComponentsJson: Loads component data from a JSON file into the ECS.
		   - m_SaveComponentsJson: Saves component data from the ECS to a JSON file.
		   - m_SaveEntity: Serializes individual entity data to a JSON structure.
		   - m_LoadEntity: Deserializes individual entity data from a JSON structure.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef JSONHANDLER_H
#define JSONHANDLER_H


#include "../ECS/ECS.h"
#include "../Dependencies/rapidjson/document.h"

#include <filesystem>

namespace Serialization {
	class Serialize {
	public:

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadComponentsJson(const std::string& jsonFilePath)
			\brief     Loads component data from a JSON file into the ECS framework.
			\param[in] jsonFilePath The path to the JSON file containing the component data.
			\details   This function reads the specified JSON file and parses component data for various components such as
					   `TransformComponent`, `MovementComponent`, `ColliderComponent`, `PlayerComponent`, and others.
					   The data is then applied to the appropriate entities in the ECS.
		*/
		/******************************************************************/
		static void m_LoadComponentsJson(const std::filesystem::path& jsonFilePath);

		/******************************************************************/
		/*!
			\fn        Serialize::m_SaveComponentsJson(const std::string& filePath, const std::unordered_map<ecs::EntityID, std::bitset<ecs::ComponentType::TOTALTYPECOMPONENT>>& ECS_EntityMap, const std::vector<std::string>& objTextEntries, const std::vector<ecs::EntityID>& objEntityId)
			\brief     Saves component data from the ECS to a JSON file.
			\param[in] filePath      The path to save the JSON file.
			\param[in] ECS_EntityMap The map of entities and their associated components.
			\param[in] objTextEntries The list of entity names to save.
			\param[in] objEntityId   The list of entity IDs to save.
			\details   This function iterates through the ECS entity map and saves relevant component data such as `TransformComponent`,
					   `MovementComponent`, `ColliderComponent`, `PlayerComponent`, and more. The saved data is written to a JSON file
					   for later retrieval and reloading.
		*/
		/******************************************************************/
		static void m_SaveComponentsJson(const std::filesystem::path& filePath);

		/******************************************************************/
		/*!
			\fn        Serialize::m_SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities)
			\brief     Saves an individual entity's component data to a JSON structure.
			\param[in] entityId       The ID of the entity to be saved.
			\param[out] parentArray   The JSON value to which the entity data will be appended.
			\param[in] allocator      The allocator used for JSON memory management.
			\param[in,out] savedEntities A set that keeps track of saved entities to avoid redundant saves.
			\details   This function serializes components such as `TransformComponent`, `CameraComponent`, `ScriptComponent`, and others.
					   It appends the serialized entity data to the given JSON array, allowing hierarchical saving of parent-child relationships.
		*/
		/******************************************************************/
		static void m_SaveEntity(ecs::EntityID entityId, rapidjson::Value& parentArray, rapidjson::Document::AllocatorType& allocator, std::unordered_set<ecs::EntityID>& savedEntities);

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName)
			\brief     Loads an individual entity's component data from a JSON structure.
			\param[in] entityData     The JSON value containing the entity data.
			\param[in] parentID       The optional ID of the parent entity, used to maintain parent-child relationships.
			\param[in] sceneName      The name of the scene to which this entity belongs.
			\details   This function parses component data from a JSON object and assigns it to the entity in the ECS. Components such as
					   `TransformComponent`, `CameraComponent`, `ScriptComponent`, `TextComponent`, `SpriteComponent`, etc., are reconstructed.
		*/
		/******************************************************************/
		static void m_LoadEntity(const rapidjson::Value& entityData, std::optional<ecs::EntityID> parentID, const std::string& sceneName);

		/******************************************************************/
		/*!
			\fn        Serialize::m_LoadConfig()
			\brief     Loads configuration settings such as window dimensions and FPS cap from a config file.
			\details   This function reads the configuration file `Config.txt`, extracting window height, width, and FPS cap values,
					   and assigns them to the corresponding variables in the `Helper::Helpers` class.
		*/
		/******************************************************************/
		static void m_LoadConfig();

		/******************************************************************/
		/*!
			\fn        Serialize::m_JsonFileValidation(const std::string& filePath)
			\brief     Validates if the JSON file exists and creates a new one if it doesn't.
			\param[in] filePath The path to the JSON file to be validated.
			\details   This function checks if the specified JSON file exists. If it does not, it creates a new JSON file and initializes
					   it with an empty array structure (`[]`).
		*/
		/******************************************************************/
		static void m_JsonFileValidation(const std::string& filePath);
	};
}



#endif JSONHANDLER_H

