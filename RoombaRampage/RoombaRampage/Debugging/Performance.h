#ifndef PERFORMANCE_H
#define PERFORMANCE_H
#include <chrono>
#include <format>
#include <source_location>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <../ECS/System/SystemType.h>

namespace PerformanceTracker{
	class Performance {
	private:
		float engineTime = 0.0f;
		int totalSystem = Ecs::TotalTypeSystem;
		std::vector<std::pair<Ecs::TypeSystem, float>> systemTimeList;
		std::string typeToString(Ecs::TypeSystem);
	public:
		Performance();
		void printPerformance();
		void printFPS(float);
		void resetPerformance();
		void addTime(float);

		void addPair(Ecs::TypeSystem, float);
	};
}

#endif
