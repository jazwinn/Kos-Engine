#include "../ECS/ECS.h"

namespace Serialization {
	class Serialize {
	public:
		static void LoadComponentsJson(const std::string& jsonFilePath, Ecs::ECS* ecs, std::vector<std::string>& obj_text_entries);

		static void SaveComponentsJson(const std::string& filePath, const std::unordered_map<Ecs::EntityID, std::bitset<Ecs::ComponentType::TotalTypeComponent>>& ECS_EntityMap, const std::vector<std::string>& obj_text_entries, const std::vector<Ecs::EntityID>& obj_entity_id);

		static void LoadConfig();
	};
}