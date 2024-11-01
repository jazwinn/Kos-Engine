/*!
\file      LogicSystem.cpp
\author    
\par       
\date      Sept 28, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../ECS.h"

#include "LogicSystem.h"
#include "../Asset Manager/AssetManager.h"


namespace ecs {

	void LogicSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecScriptComponentPtr.begin(), m_vecScriptComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecScriptComponentPtr.end()) {
			m_vecScriptComponentPtr.push_back((ScriptComponent*)ecs->m_ECS_CombinedComponentPool[TYPESCRIPTCOMPONENT]->m_GetEntityComponent(ID));
			
		}

	}

	void LogicSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecScriptComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				return;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecScriptComponentPtr.size() - 1;
		std::swap(m_vecScriptComponentPtr[IndexID], m_vecScriptComponentPtr[IndexLast]);

		//popback the vector;
		m_vecScriptComponentPtr.pop_back();

	}

	void LogicSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPESCRIPTCOMPONENT);

	}

	void LogicSystem::m_Update() {

		//ECS* ecs = ECS::m_GetInstance();
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		//if (m_vecMovementComponentPtr.size() != m_vecTransformComponentPtr.size()) {
		//	//std::cout << "Error: Vectors container size does not Match" << std::endl;
		//	LOGGING_ERROR("Error: Vectors container size does not Match");
		//	return;
		//}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecScriptComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			ScriptComponent* scriptComp = m_vecScriptComponentPtr[n];
			//EntityID id = scriptComp->m_Entity;

			for (const std::string& script : scriptComp->m_scripts) {

			// run the scripts update fuction
				assetManager->m_scriptManager.m_InvokeMethod(script, script, "Update", nullptr, 0);

			}
		}
	}

}

