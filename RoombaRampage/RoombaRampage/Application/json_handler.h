
#include "../ECS/ECS.h"

void LoadComponentsJson(const std::string& jsonFilePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc);

void SaveComponentsJson(const std::string& jsonFilePath, Ecs::TransformComponent* tc, Ecs::MovementComponent* mc);