#include "../Config/pch.h"
#include "Physics.h"
#include "../ECS/ECS.h"
#include "PhysicsCollisionResponse.h"

namespace physicspipe {

	void m_FindContactPoints() {
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();

		for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
			std::shared_ptr<physicspipe::PhysicsData> entA = vecCollisionEntityPair[i].first;
			std::shared_ptr<physicspipe::PhysicsData> entB = vecCollisionEntityPair[i].second;
			ecs::EntityID first = entA.get()->m_ID;
			ecs::EntityID second = entB.get()->m_ID;
			const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(first));
			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(second));
			if (entA.get()->m_GetEntity() == EntityType::RECTANGLE && entB.get()->m_GetEntity() == EntityType::RECTANGLE) {
				std::tuple<int, vector2::Vec2, vector2::Vec2> contact = m_FindSquareSquareContact(static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices());
				
				if (std::get<0>(contact) == 1) {
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first);
				}
				else {
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first);
					colComp->m_contactPoints.emplace_back(std::get<2>(contact), second);
					colComp2->m_contactPoints.emplace_back(std::get<2>(contact), first);
				}
			}
			else if (entA.get()->m_GetEntity() == EntityType::RECTANGLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second);
				colComp2->m_contactPoints.emplace_back(contact, first);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::RECTANGLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second);
				colComp2->m_contactPoints.emplace_back(contact, first);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleCirleContact(entA.get()->m_position, static_cast<physicspipe::Circle*>(entA.get())->m_radius, entB.get()->m_position);
				colComp->m_contactPoints.emplace_back(contact,second);
				colComp2->m_contactPoints.emplace_back(contact, first);
			}
		}
	}

	vector2::Vec2 m_FindCircleCirleContact(const vector2::Vec2& cirleAPos, const float& cirlceARadius, const vector2::Vec2& circleBPos) {
		vector2::Vec2 ab = circleBPos - cirleAPos;
		vector2::Vec2 dir = {};
		vector2::Vec2 ret{};
		vector2::Vec2::m_funcVec2Normalize(dir, ab);
		ret = cirleAPos + dir * cirlceARadius;
		return ret;

	}

	std::pair<vector2::Vec2, float> m_PointSegDist(const vector2::Vec2& circlePos, const vector2::Vec2& startingPoint, const vector2::Vec2& endPoint) {
		vector2::Vec2 lineSeg = endPoint - startingPoint;
		vector2::Vec2 startToCircle = circlePos - startingPoint;
		std::pair<vector2::Vec2, float> ret;
		float proj = vector2::Vec2::m_funcVec2DDotProduct(startToCircle, lineSeg);
		float lineSegLenSq = vector2::Vec2::m_funcVec2DSquareLength(lineSeg);
		float dist = proj / lineSegLenSq;
		dist = std::max(0.f, std::min(1.f, dist));
		if (lineSegLenSq <= 0.0005f) {
			ret.first = startingPoint;
		}
		else if (dist<= 0.0005f) {
			ret.first = startingPoint;
		}
		else if (dist >= 1.f) {
			ret.first = endPoint;
			
		}
		else {
			ret.first = startingPoint + lineSeg * dist;
		}
		//ret.second = (ret.first.m_x - circlePos.m_x) * (ret.first.m_x - circlePos.m_x) + (ret.first.m_y - circlePos.m_y) * (ret.first.m_y - circlePos.m_y);
		ret.second = vector2::Vec2::m_funcVec2DSquareDistance(circlePos, ret.first);
		return ret;
	}

	vector2::Vec2 m_FindCircleSquareContact(const vector2::Vec2& circlePos, const std::vector<vector2::Vec2>& vertices) {

		float minDistSq = std::numeric_limits<float>::max();
		vector2::Vec2 contactRet{};
		for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
			vector2::Vec2 va = vertices[i];
			vector2::Vec2 vb = vertices[(i + 1) % vertices.size()];
			std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(circlePos, va, vb);

			if (optionOfContact.second < minDistSq) {
				minDistSq = optionOfContact.second;
				contactRet = optionOfContact.first;
			}
		}
		return contactRet;
	}
	
	bool m_AlmostEqual(float lhs, float rhs) {
		const float minDist = 0.0005f;
		return abs(lhs - rhs) < minDist;
	}

	bool m_AlmostEqual(vector2::Vec2 lhs, vector2::Vec2 rhs) {
		return m_AlmostEqual(lhs.m_x, rhs.m_x) && m_AlmostEqual(lhs.m_y, rhs.m_y);
	}

	std::tuple<int, vector2::Vec2, vector2::Vec2> m_FindSquareSquareContact(const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& verticesB) {
		vector2::Vec2 contact1 = { 0,0 };
		vector2::Vec2 contact2 = { 0,0 };
		int points = 0;
		float minDistSq = std::numeric_limits<float>::max();
		for (int i = 0; i < static_cast<int>(verticesA.size()); i++) {
			vector2::Vec2 pointA = verticesA[i];
			for (int j = 0; j < static_cast<int>(verticesB.size()); j++) {
				vector2::Vec2 va = verticesB[j];
				vector2::Vec2 vb = verticesB[(j + 1) % static_cast<int>(verticesB.size())];

				std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(pointA, va, vb);

				if (m_AlmostEqual(optionOfContact.second, minDistSq)) {
					if (!m_AlmostEqual(contact1, optionOfContact.first) && !m_AlmostEqual(contact2, optionOfContact.first)) {
						contact2 = optionOfContact.first;
						points = 2;
					}
				}
				else if (optionOfContact.second < minDistSq) {
					minDistSq = optionOfContact.second;
					contact1 = optionOfContact.first;
					points = 1;
				}

			}
		}

		for (int i = 0; i < static_cast<int>(verticesB.size()); i++) {
			vector2::Vec2 pointB = verticesB[i];
			for (int j = 0; j < static_cast<int>(verticesA.size()); j++) {
				vector2::Vec2 va = verticesA[j];
				vector2::Vec2 vb = verticesA[(j + 1) % static_cast<int>(verticesA.size())];

				std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(pointB, va, vb);

				if (m_AlmostEqual(optionOfContact.second, minDistSq)) {
					if (!m_AlmostEqual(contact1, optionOfContact.first) && !m_AlmostEqual(contact2, optionOfContact.first)) {
						contact2 = optionOfContact.first;
						points = 2;
					}
				}
				else if (optionOfContact.second < minDistSq) {
					minDistSq = optionOfContact.second;
					contact1 = optionOfContact.first;
					points = 1;
				}

			}
		}
		std::tuple<int, vector2::Vec2, vector2::Vec2> contactRet = { points, contact1, contact2 };
		return contactRet;
	}
}