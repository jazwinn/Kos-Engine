#include "../ECS/ECS.h"





namespace Serialization{
	class Serialize {
	public:
		static void LoadComponentsJson(const std::string& jsonFilePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc);

		static void SaveComponentsJson(const std::string& filePath, const std::unordered_map<Ecs::EntityID, std::bitset<Ecs::ComponentType::TotalTypeComponent>>& ECS_EntityMap);

		static void LoadConfig();
	};
}