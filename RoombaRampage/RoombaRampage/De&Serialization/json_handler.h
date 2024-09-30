#include "../ECS/ECS.h"

namespace Serialization {
	class Serialize {
	public:
		static void LoadComponentsJson(const std::string& jsonFilePath, ecs::ECS* ecs, std::vector<std::string>& obj_text_entries);

		static void SaveComponentsJson(const std::string& filePath, const std::unordered_map<ecs::EntityID, std::bitset<ecs::ComponentType::TOTALTYPECOMPONENT>>& ECS_EntityMap, const std::vector<std::string>& obj_text_entries, const std::vector<ecs::EntityID>& obj_entity_id);

		static void LoadConfig();
	};
}