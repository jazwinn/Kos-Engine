#include "Performance.h"

namespace PerformanceTracker {
	Performance::Performance() {
		engineTime = 0.0f; systemTimeList = {};
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
			return "Error";
		}
	}
	void Performance::addTime(float time) {
		engineTime += time;
	}

	void Performance::addPair(Ecs::TypeSystem system, float time) {
		systemTimeList.push_back(std::pair(system, time));
	}
	void Performance::printPerformance(){
		std::cout << "###########################################" << std::endl;
		for (auto& val : systemTimeList) {
			std::string system = typeToString(val.first);
			float systemTime = (val.second / engineTime) * 100;
			std::cout << "Time taken for " << system << ": " << val.second << std::endl;
			std::cout << "Percentage overall: " << systemTime << std::endl;
		}
		float avgTime = engineTime / totalSystem;
		std::cout << "Average time:" << avgTime << std::endl;
		std::cout << "###########################################" << std::endl;
	}

	void Performance::resetPerformance() {
		engineTime = 0.0f;
		systemTimeList.clear();
	}
}