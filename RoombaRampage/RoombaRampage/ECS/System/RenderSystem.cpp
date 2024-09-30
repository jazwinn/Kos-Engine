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
			vecColliderComponentPtr.push_back((ColliderComponent*)ecs->ECS_CombinedComponentPool[TypeColliderComponent]->GetEntityComponent(ID));
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
		std::swap(vecColliderComponentPtr[IndexID], vecColliderComponentPtr[IndexLast]);

		//popback the vector;
		vecSpriteComponentPtr.pop_back();
		vecTransformComponentPtr.pop_back();
		vecColliderComponentPtr.pop_back();
	}

	void RenderSystem::Init()
	{
		SystemSignature.set(TypeTransformComponent);
		SystemSignature.set(TypeSpriteComponent);
		SystemSignature.set(TypeColliderComponent);
	}

	void RenderSystem::Update()
	{
		//ECS* ecs = ECS::GetInstance();
		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

		if (vecSpriteComponentPtr.size() != vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < vecSpriteComponentPtr.size(); n++) {
			TransformComponent* transform = vecTransformComponentPtr[n];
			SpriteComponent* sprite = vecSpriteComponentPtr[n];
			ColliderComponent* ColComp = vecColliderComponentPtr[n];

			graphicsPipe->modelData.push_back({ transform->rotation, glm::vec2{transform->scale.m_x, transform->scale.m_y}, glm::vec3{transform->position.m_x,transform->position.m_y, 0} ,sprite->imageID, 0, 0 });

			if (ColComp->drawDebug)
			{
				graphicsPipe->debugBoxData.push_back({ 0, glm::vec2{ColComp->Size.m_x * transform->scale.m_x, ColComp->Size.m_y * transform->scale.m_y}, glm::vec3{transform->position.m_x + ColComp->OffSet.m_x,transform->position.m_y + ColComp->OffSet.m_y, 0} ,ColComp->isCollided, 0 });
			}
			
		}


	}


}

