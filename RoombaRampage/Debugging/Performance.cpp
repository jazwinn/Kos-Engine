/******************************************************************/
/*!
\file      Performance.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Performance functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "Performance.h"

namespace performancetracker {
	//Global Varaibles
	float Performance::m_engineTime = 0.f;
	int Performance::m_totalSystem = ecs::TOTALTYPESYSTEM;
	std::unordered_map<ecs::TypeSystem, std::pair<std::string, float>> Performance::m_systemTimeList;

	Performance::Performance() {
		//engineTime = 0.0f;
	}
	std::string Performance::m_typeToString(ecs::TypeSystem type) {
		switch (type) {
		case ecs::TypeSystem::TYPEMOVEMENTSYSTEM:
			return "Movement System";
		case ecs::TypeSystem::TYPECONTROLSYSTEM:
			return "Control System";
		case ecs::TypeSystem::TYPECOLLISIONSYSTEM:
			return "Collision System";
		case ecs::TypeSystem::TYPECOLLISIONRESPONSESYSTEM:
			return "Collision Response System";
		case ecs::TypeSystem::TYPEDEBUGDRAWINGSYSTEM:
			return "Render Debug System";
		case ecs::TypeSystem::TYPERENDERTEXTSYSTEM:
			return "Render Text System";
		case ecs::TypeSystem::TYPERENDERSYSTEM:
			return "Render System";
		default:
			return "Error (Add System to Performance.cpp)";
		}
	}

	void Performance::m_ResetTotalSystemTime() {
		m_engineTime = 0;
	}
	void Performance::m_UpdateTotalSystemTime(float time) {
		m_engineTime += time;
	}

	void Performance::m_AddSystem(ecs::TypeSystem System) {
		m_systemTimeList[System] = { m_typeToString(System),0};
	}

	void Performance::m_UpdateSystemTime(ecs::TypeSystem system, float time) {
		m_systemTimeList[system].second = time;
	}

	float Performance::m_GetTotalSystemTime() {
		return m_engineTime;
	}
	float Performance::m_GetSystemTime(ecs::TypeSystem sys) {
		return m_systemTimeList[sys].second;
	}
	
	std::string Performance::m_GetSystemString(ecs::TypeSystem Sys) {
		return m_systemTimeList[Sys].first;
	}

	void Performance::m_PrintPerformance(){
		//std::cout << "###########################################" << std::endl;
		//for (auto& val : systemTimeList) {
		//	std::string system = typeToString(val.first);
		//	float systemTime = (val.second / engineTime) * 100;
		//	std::cout << "Time taken for " << system << ": " << val.second << std::endl;
		//	std::cout << "Percentage overall: " << systemTime << std::endl;
		//}
		//float avgTime = engineTime / totalSystem;
		//std::cout << "Average time:" << avgTime << std::endl;
		//std::cout << "###########################################" << std::endl;
	}

	void Performance::m_PrintFPS(float dt) {
		std::cout << "FPS: " << 1 / dt << std::endl;
	}

}