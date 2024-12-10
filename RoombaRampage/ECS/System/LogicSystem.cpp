/******************************************************************/
/*!
\file      LogicSystem.h
\author    Ng Jaz Winn
\par       jazwinn.ng@digipen.edu
\date      Nov 30, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
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
				break;
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

	void LogicSystem::m_StartLogic()
	{
		ECS* ecs = ECS::m_GetInstance();

		//reset all script instance
		std::shared_ptr<ComponentPool<ScriptComponent>> pool = std::dynamic_pointer_cast<ComponentPool<ScriptComponent>>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]);
		std::for_each(pool->m_Pool.begin(), pool->m_Pool.end(), [](auto& x) {x.m_scriptInstances.clear(); });
		
		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecScriptComponentPtr.size(); n++) {
			ScriptComponent* scriptComp = m_vecScriptComponentPtr[n];

			//skip prefab
			if (ecs->m_ECS_SceneMap.find(scriptComp->m_scene)->second.m_isPrefab)continue;

			CreateandStartScriptInstance(scriptComp);

		}
	}


	void LogicSystem::CreateandStartScriptInstance(ecs::ScriptComponent* scriptComp) {
		// create instance for each script

		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		for (const std::string& _script : scriptComp->m_scripts) {

			//if (assetManager->m_scriptManager.m_ScriptMap.find(_script) == assetManager->m_scriptManager.m_ScriptMap.end()) {
			//	LOGGING_ERROR("SCRIPT NOT FOUND ! PLEASE RELAUNCH APPLIATION");
			//	continue;
			//}

			// retieve isntance for each object
			//std::cout << _script << std::endl;
			scriptComp->m_scriptInstances[_script] = assetManager->m_scriptManager.m_CreateObjectInstance("LogicScript", _script);
		}

		// invoke start function
		for (auto& instance : scriptComp->m_scriptInstances) {

			void* params[1];
			params[0] = &scriptComp->m_Entity; // Pass the entity ID

			assetManager->m_scriptManager.m_InvokeMethod(instance.first, "GetEntityID", instance.second, params);
			assetManager->m_scriptManager.m_InvokeMethod(instance.first, "Start", instance.second, nullptr);

		}
	}

	void LogicSystem::m_Update(const std::string& scene) {

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
			
			//skip component not of the scene
			if (scriptComp->m_scene != scene) continue;


			//check if scriptcomponent have instance
			for (auto& scriptstring : scriptComp->m_scripts) {
				if (scriptComp->m_scriptInstances.find(scriptstring) == scriptComp->m_scriptInstances.end()) {
					CreateandStartScriptInstance(scriptComp);
					LOGGING_INFO("Script Instance Created");
					break;
				}
			}


			for (auto& script : scriptComp->m_scriptInstances) {
				try {
					// run the scripts update fuction
					assetManager->m_scriptManager.m_InvokeMethod(script.first, "Update", script.second, nullptr);
				}
				catch (...) {
					break;
				}


			}
		}
	}

}

