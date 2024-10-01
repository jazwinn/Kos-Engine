/********************************************************************/
/*!
\file      json_handler.h
\author    Chiu Jun Jie, junjie.c , 2301524 / Jaz
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This header file declares functions that handle JSON-related operations for component serialization and
		   deserialization in the ECS framework.
		   - LoadConfig: Loads configuration settings such as window dimensions and FPS cap from a config file.
		   - JsonFileValidation: Validates if the JSON file exists and creates a new one if it doesn't.
		   - LoadComponentsJson: Loads component data from a JSON file into the ECS.
		   - SaveComponentsJson: Saves component data from the ECS to a JSON file.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../ECS/ECS.h"

namespace Serialization {
	class Serialize {
	public:
		static void LoadComponentsJson(const std::string& jsonFilePath, ecs::ECS* ecs, std::vector<std::string>& obj_text_entries);

		static void SaveComponentsJson(const std::string& filePath, const std::unordered_map<ecs::EntityID, std::bitset<ecs::ComponentType::TOTALTYPECOMPONENT>>& ECS_EntityMap, const std::vector<std::string>& obj_text_entries, const std::vector<ecs::EntityID>& obj_entity_id);

		static void LoadConfig();

		static void JsonFileValidation(const std::string& filePath);
	};
}