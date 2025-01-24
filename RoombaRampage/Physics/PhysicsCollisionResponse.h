#pragma once
#include "Physics.h"
#include "../ECS/ECS.h"

namespace physicspipe {
	enum CollisionState {
		ENTERED,
		CONTINUOUS,
		EXIT
	};

	class CollisionResponseData {
	public:
		std::pair<vector2::Vec2, ecs::EntityID> m_contactPointEnt{};
		int m_numOfContacts;
		CollisionResponseData(vector2::Vec2 inVec, ecs::EntityID inID, int inNum) {
			m_contactPointEnt.first = inVec;
			m_contactPointEnt.second = inID;
			m_numOfContacts = inNum;
		}
	};


	void m_FindContactPoints();

	void m_FindCollisionFlags();

	std::pair<int, int> m_FindCircleCircleFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, ecs::EntityID entA, ecs::EntityID entB, const vector2::Vec2& centerA, const float& radA, const vector2::Vec2& centerB, const float& radB, const vector2::Vec2& dirVecA, const vector2::Vec2& dirVecB);

	std::pair<int, int> m_FindCircleSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, ecs::EntityID entA, ecs::EntityID entB, const vector2::Vec2& centerA, const float& radA, const vector2::Vec2& dirVecA,const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB);

	std::pair<int,int> m_FindSquareSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, ecs::EntityID entA, ecs::EntityID entB,const vector2::Vec2& centerA, const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& edgesA, const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB);

	std::pair<vector2::Vec2, float> m_PointSegDist(const vector2::Vec2& circlePos, const vector2::Vec2& startingPoint, const vector2::Vec2& endPoint);

	vector2::Vec2 m_FindCircleCirleContact(const vector2::Vec2& cirleAPos, const float& cirlceARadius, const vector2::Vec2& circleBPos);

	vector2::Vec2 m_FindCircleSquareContact(const vector2::Vec2& circlePos, const std::vector<vector2::Vec2>& vertices);

	std::tuple<int, vector2::Vec2, vector2::Vec2> m_FindSquareSquareContact(const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& verticesB);

	//class PhysicsCollide {
	//	CollisionState m_state;
	//	ecs::EntityID m_entID;
	//	ecs::EntityID m_entColidedID;

	//	void m_OnColEnter();
	//	void m_OnColStay();
	//	void m_OnColExit();
	//};

	//class PhysicsCollisionResponse {
	//public:
	//	void m_CollisionUpdate();
	//};
}