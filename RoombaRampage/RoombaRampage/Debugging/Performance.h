/******************************************************************/
/*!
\file      Performance.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Performance functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

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

namespace performancetracker{
	class Performance {
	public:
		Performance();

		static void m_ResetTotalSystemTime();
		static void m_AddSystem(Ecs::TypeSystem System);

		static void m_PrintPerformance();
		static void m_PrintFPS(float);
		static void m_UpdateTotalSystemTime(float);

		static float m_GetTotalSystemTime();
		static float m_GetSystemTime(Ecs::TypeSystem);
		static std::string m_GetSystemString(Ecs::TypeSystem);
		static void m_UpdateSystemTime(Ecs::TypeSystem, float);
	private:
		static std::string m_typeToString(Ecs::TypeSystem);
		static float m_engineTime;
		static int m_totalSystem;
		static std::unordered_map<Ecs::TypeSystem, std::pair<std::string,float>> m_systemTimeList;
		

	};
}

#endif
