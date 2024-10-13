
#include "Hierachy.h"

namespace ecs {

	void Hierachy::m_SetParent(EntityID parent, EntityID child) {

		ECS* ecs = ECS::m_GetInstance();

		m_RemoveParent(child);

		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		//checks if child is already in parent
		if (m_GetParent(child).has_value()) {
			return;
		}

		//checks if parent is getting dragged into its child
		EntityID id = parent;
		while (m_GetParent(id).has_value()) {
			EntityID checkParentid = m_GetParent(id).value();
			if (checkParentid == child) {
				LOGGING_WARN("Cannot assign parent to its own child");
				return;
			}
			id = checkParentid;

		}


		parentTransform->m_childID.push_back(child);

		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = true;
		childTransform->m_parentID = parent;
	}

	void Hierachy::m_RemoveParent(EntityID child) {
		// removes id from both the child and the parents vector
		ECS* ecs = ECS::m_GetInstance();

		if (!m_GetParent(child).has_value()) {
			// does not have parrent
			return;
		}

		EntityID parent = m_GetParent(child).value();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		size_t pos{};
		for (EntityID& id : parentTransform->m_childID) {
			if (child == id) {
				parentTransform->m_childID.erase(parentTransform->m_childID.begin() + pos);
				break;
			}
			pos++;
		}


		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		childTransform->m_haveParent = false;
		childTransform->m_parentID = 0;
	}

	std::optional<EntityID> Hierachy::m_GetParent(EntityID child)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* childTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(child);
		if (!childTransform->m_haveParent) {
			return std::optional<EntityID>();
		}

		return childTransform->m_parentID;

	}

	std::optional<std::vector<EntityID>>Hierachy::m_GetChild(EntityID parent)
	{
		ECS* ecs = ECS::m_GetInstance();
		TransformComponent* parentTransform = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(parent);
		if (parentTransform->m_childID.size() <= 0) {
			return std::optional<std::vector<EntityID>>();
		}

		return parentTransform->m_childID;

	}

}