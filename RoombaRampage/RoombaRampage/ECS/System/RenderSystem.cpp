#include "../ECS.h"

#include "RenderSystem.h"
#include "../ECS/Component/SpriteComponent.h"
#include "../Graphics/GraphicsPipe.h"

void RenderSystem::RegisterSystem(EntityID ID) {
	ECS* ecs = ECS::GetInstance();

	

	//Step 1 : check
	bool store = true;
	for (auto& TransformComponentPtr : vecTransformComponentPtr) {
		if (TransformComponentPtr->Entity == ID) {
			store = false;
			break;
		}
		else {
			store = true;
		}
	}
	//Step 2 : Store address into vector
	if (store == true) {
		vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
	}

	//check again
	for (auto& SpriteComponentPtr : vecSpriteComponentPtr) {
		if (SpriteComponentPtr->Entity == ID) {
			store = false;
			break;
		}
		else {
			store = true;
		}
	}
	if (store) {
		vecSpriteComponentPtr.push_back((SpriteComponent*)ecs->ECS_CombinedComponentPool[TypeSpriteComponent]->GetEntityComponent(ID));
	}


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

		//sprite not need currently
		//SpriteComponent* MovComp = vecSpriteComponentPtr[n];
		TransformComponent* transform = vecTransformComponentPtr[n];
		graphicsPipe->modelData.push_back({ transform->roation, {transform->scale, transform->scale}, {transform->position.x,transform->position.y,0} });
	}



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
