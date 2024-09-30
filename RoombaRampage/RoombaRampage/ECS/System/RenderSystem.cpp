#include "../ECS.h"

#include "RenderSystem.h"
#include "../ECS/Component/SpriteComponent.h"
#include "../Graphics/GraphicsPipe.h"

namespace Ecs {

	void RenderSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		if (std::find_if(vecTransformComponentPtr.begin(), vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecTransformComponentPtr.end()) {
			vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
			vecSpriteComponentPtr.push_back((SpriteComponent*)ecs->ECS_CombinedComponentPool[TypeSpriteComponent]->GetEntityComponent(ID));
		}

	}

	void RenderSystem::DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& SpriteComponentPtr : vecSpriteComponentPtr) {
			if (SpriteComponentPtr->Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = vecSpriteComponentPtr.size() - 1;
		std::swap(vecSpriteComponentPtr[IndexID], vecSpriteComponentPtr[IndexLast]);
		std::swap(vecTransformComponentPtr[IndexID], vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		vecSpriteComponentPtr.pop_back();
		vecTransformComponentPtr.pop_back();
	}

	void RenderSystem::Init()
	{
		SystemSignature.set(TypeTransformComponent);
		SystemSignature.set(TypeSpriteComponent);
	}

	void RenderSystem::Update()
	{
		ECS* ecs = ECS::GetInstance();
		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

		if (vecSpriteComponentPtr.size() != vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < vecSpriteComponentPtr.size(); n++) {

			//std::cout << "Update Entity: " << n << std::endl;
			//sprite not need currently
			//SpriteComponent* MovComp = vecSpriteComponentPtr[n];
			TransformComponent* transform = vecTransformComponentPtr[n];
			SpriteComponent* sprite = vecSpriteComponentPtr[n];

			graphicsPipe->modelData.push_back({ transform->rotation, glm::vec2{transform->scale.x, transform->scale.y}, glm::vec3{transform->position.x,transform->position.y, 0} ,sprite->imageID, 0, 0 });
			
		}


	}


}

