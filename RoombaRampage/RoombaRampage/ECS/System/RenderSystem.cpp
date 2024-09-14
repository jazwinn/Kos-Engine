#include "../ECS.h"

#include "RenderSystem.h"
#include "../ECS/Component/SpriteComponent.h"
#include "../Graphics/GraphicsPipe.h"

void RenderSystem::Init()
{
	
}

void RenderSystem::Update()
{
}

void RenderSystem::funcTransferData()
{
	ECS* ecs = ECS::GetInstance();
	GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

	//Loops Through all the entity
	for (EntityID ID{}; ID < ecs->EntityCount; ID++) {

		//IS THERE A MORE EFFICIENT WAY?? ASK KITSON

		if (ecs->ECS_CombinedComponentPool[TypeTransformComponent]->HasComponent(ID) && ecs->ECS_CombinedComponentPool[TypeSpriteComponent]->HasComponent(ID))
		{
			TransformComponent *transform = static_cast<TransformComponent*>(ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
			graphicsPipe->modelData.push_back({ transform->roation, {transform->scale, transform->scale}, {transform->position.x,transform->position.y,0} });
		}


	}
}
