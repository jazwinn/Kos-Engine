#ifndef PERFORMANCE_H
#define PERFORMANCE_H
#include <chrono>
#include <format>
#include <source_location>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <utility>
#include <../ECS/System/SystemType.h>

namespace PerformanceTracker{
	class Performance {

		
		static std::string typeToString(Ecs::TypeSystem);

	public:
		Performance();

		static void ResetTotalSystemTime();
		static void AddSystem(Ecs::TypeSystem System);

		static void printPerformance();
		static void printFPS(float);
		static void UpdateTotalSystemTime(float);

		static float GetTotalSystemTime();
		static float getSystemTime(Ecs::TypeSystem);
		static std::string getSystemString(Ecs::TypeSystem);
		static void UpdateSystemTime(Ecs::TypeSystem, float);
	private:
		
		static float engineTime;
		static int totalSystem;
		static std::unordered_map<Ecs::TypeSystem, std::pair<std::string,float>> systemTimeList;
		

	};
}

#endif
