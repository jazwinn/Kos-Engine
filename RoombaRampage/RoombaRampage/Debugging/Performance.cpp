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

namespace PerformanceTracker {
	//Global Varaibles
	float Performance::engineTime = 0.f;
	int Performance::totalSystem = Ecs::TotalTypeSystem;
	std::unordered_map<Ecs::TypeSystem, std::pair<std::string, float>> Performance::systemTimeList;

	Performance::Performance() {
		//engineTime = 0.0f;
	}
	std::string Performance::typeToString(Ecs::TypeSystem type) {
		switch (type) {
		case Ecs::TypeSystem::TypeMovementSystem:
			return "Movement System";
		case Ecs::TypeSystem::TypeControlSystem:
			return "Control System";
		case Ecs::TypeSystem::TypeCollisionSystem:
			return "Collision System";
		case Ecs::TypeSystem::TypeCollisionResponseSystem:
			return "Collision Response System";
		case Ecs::TypeSystem::TypeRenderSystem:
			return "Render System";
		default:
			return "Error (Add System to Performance.cpp)";
		}
	}

	void Performance::ResetTotalSystemTime() {
		engineTime = 0;
	}
	void Performance::UpdateTotalSystemTime(float time) {
		engineTime += time;
	}

	void Performance::AddSystem(Ecs::TypeSystem System) {
		systemTimeList[System] = {typeToString(System),0};
	}

	void Performance::UpdateSystemTime(Ecs::TypeSystem system, float time) {
		systemTimeList[system].second = time;
	}

	float Performance::GetTotalSystemTime() {
		return engineTime;
	}
	float Performance::getSystemTime(Ecs::TypeSystem sys) {
		return systemTimeList[sys].second;
	}
	
	std::string Performance::getSystemString(Ecs::TypeSystem Sys) {
		return systemTimeList[Sys].first;
	}

	void Performance::printPerformance(){
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

	void Performance::printFPS(float dt) {
		std::cout << "FPS: " << 1 / dt << std::endl;
	}

}