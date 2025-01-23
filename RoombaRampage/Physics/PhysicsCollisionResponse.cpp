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
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second,1);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first,1);
				}
				else {
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second,2);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first,2);
					colComp->m_contactPoints.emplace_back(std::get<2>(contact), second,2);
					colComp2->m_contactPoints.emplace_back(std::get<2>(contact), first,2);
				}
			}
			else if (entA.get()->m_GetEntity() == EntityType::RECTANGLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::RECTANGLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleCirleContact(entA.get()->m_position, static_cast<physicspipe::Circle*>(entA.get())->m_radius, entB.get()->m_position);
				colComp->m_contactPoints.emplace_back(contact,second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
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

	bool m_AlmostEqualCP(float lhs, float rhs) {
		const float minDist = 0.01f;
		return abs(lhs - rhs) < minDist;
	}

	bool m_AlmostEqualCP(vector2::Vec2 lhs, vector2::Vec2 rhs) {
		return m_AlmostEqualCP(lhs.m_x, rhs.m_x) && m_AlmostEqualCP(lhs.m_y, rhs.m_y);
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
					if (!m_AlmostEqualCP(contact1, optionOfContact.first) && !m_AlmostEqualCP(contact2, optionOfContact.first)) {
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
					if (!m_AlmostEqualCP(contact1, optionOfContact.first) && !m_AlmostEqualCP(contact2, optionOfContact.first)) {
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

	void m_FindCollisionFlags() {
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
			std::shared_ptr<physicspipe::PhysicsData> entA = vecCollisionEntityPair[i].first;
			std::shared_ptr<physicspipe::PhysicsData> entB = vecCollisionEntityPair[i].second;
			ecs::EntityID first = entA.get()->m_ID;
			ecs::EntityID second = entB.get()->m_ID;
			const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(first));
			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(second));
			//const auto& transComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ColComp->m_Entity));
			if (colComp->m_type == EntityType::CIRCLE && colComp2->m_type == EntityType::CIRCLE) {
				//m_FindCircleCircleFlags(colComp, colComp2, colComp->m_contactPoints[i].m_contactPointEnt.first);
			}
			else if (colComp->m_type == EntityType::CIRCLE && colComp2->m_type == EntityType::RECTANGLE) {
				//m_FindCircleSquareFlags(colComp, colComp2);
			}
			else if (colComp->m_type == EntityType::RECTANGLE && colComp2->m_type == EntityType::CIRCLE) {
				//m_FindCircleSquareFlags(colComp2, colComp);
			}
			else {
				std::pair<int,int> collFlags = m_FindSquareSquareFlags(colComp->m_contactPoints,first, second, entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entA.get())->m_GetEdges(),
					entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entB.get())->m_GetEdges());
				colComp->m_blockedFlag = collFlags.first;
				colComp2->m_blockedFlag = collFlags.second;
			}
			//const auto& colComp2 = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(second));
		}
	}

	std::pair<int, int> m_FindSquareSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, ecs::EntityID entA, ecs::EntityID entB, const vector2::Vec2& centerA, const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& edgesA, const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB) {
		vector2::Vec2 topNormA{ -edgesA[0].m_x, edgesA[0].m_y };
		vector2::Vec2 rightNormA{ -edgesA[1].m_x, edgesA[1].m_y };
		vector2::Vec2 bottomNormA{ -edgesA[2].m_x, edgesA[2].m_y };
		vector2::Vec2 leftNormA{ -edgesA[3].m_x, edgesA[3].m_y };
		vector2::Vec2::m_funcVec2Normalize(topNormA, topNormA);
		vector2::Vec2::m_funcVec2Normalize(rightNormA, rightNormA);
		vector2::Vec2::m_funcVec2Normalize(bottomNormA, bottomNormA);
		vector2::Vec2::m_funcVec2Normalize(leftNormA, leftNormA);
		std::vector<vector2::Vec2> normalsA;
		normalsA.push_back(topNormA);
		normalsA.push_back(rightNormA);
		normalsA.push_back(bottomNormA);
		normalsA.push_back(leftNormA);

		vector2::Vec2 topNormB{ -edgesB[0].m_x, edgesB[0].m_y };
		vector2::Vec2 rightNormB{ -edgesB[1].m_x, edgesB[1].m_y };
		vector2::Vec2 bottomNormB{ -edgesB[2].m_x, edgesB[2].m_y };
		vector2::Vec2 leftNormB{ -edgesB[3].m_x, edgesB[3].m_y };
		vector2::Vec2::m_funcVec2Normalize(topNormB, topNormB);
		vector2::Vec2::m_funcVec2Normalize(rightNormB, rightNormB);
		vector2::Vec2::m_funcVec2Normalize(bottomNormB, bottomNormB);
		vector2::Vec2::m_funcVec2Normalize(leftNormB, leftNormB);
		std::vector<vector2::Vec2> normalsB;
		normalsB.push_back(topNormB);
		normalsB.push_back(rightNormB);
		normalsB.push_back(bottomNormB);
		normalsB.push_back(leftNormB);
		vector2::Vec2 contactPoint1;
		vector2::Vec2 contactPoint2{ 0,0 };
		int numOfContacts;
		std::pair<int,int> ret{};
		for (int i = 0; i < contactPoints.size(); i++) {
			if (contactPoints[i].m_contactPointEnt.second == entB) {
				if (contactPoints[i].m_numOfContacts == 1) {
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					numOfContacts = 1;
					break;
				}
				else if(contactPoints[i].m_numOfContacts == 2){
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					contactPoint2 = contactPoints[i + 1].m_contactPointEnt.first;
					//midPoint = (contactPoint1 + contactPoint2) / 2;
					numOfContacts = 2;
					break;
				}
			}
		}

		float min = std::numeric_limits<float>::max();
		for (int i = 0; i < verticesA.size(); i++) {
			if (numOfContacts == 2) {
				vector2::Vec2 centerToVert = verticesA[i] - centerA;
				vector2::Vec2 centerToCP = contactPoint1 - centerA;
				vector2::Vec2 centerToCP2 = contactPoint2 - centerA;
				float dotVertA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToVert);
				float dotCPA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToCP);
				float dotCPA2 = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToCP2);
				float eq = dotCPA - dotVertA;
				float eq2 = dotCPA2 - dotVertA;
				if ((m_AlmostEqual(eq,0.f) || m_AlmostEqual(eq2, 0.f)) && (eq2 < min || eq < min)) {
					min = std::min(eq, eq2);
					if (i == 0) {
						ret.first = 1;
						//break;
					}
					if (i == 1) {
						ret.first = 2;
						//break;
					}
					if (i == 2) {
						ret.first = 4;
						//break;
					}
					if (i == 3) {
						ret.first = 8;
						//break;
					}
				}
			}
			else {
				if (m_AlmostEqualCP(contactPoint1, verticesA[i])) {
					if (i == 0) {
						ret.first = 9;
						break;
					}
					if (i == 1) {
						ret.first = 3;
						break;
					}
					if (i == 2) {
						ret.first = 6;
						break;
					}
					if (i == 3) {
						ret.first = 12;
						break;
					}
				}
			}
		}
		min = std::numeric_limits<float>::max();
		for (int i = 0; i < verticesB.size(); i++) {
			if (numOfContacts == 2) {
				vector2::Vec2 centerToVert = verticesB[i] - centerB;
				vector2::Vec2 centerToCP = contactPoint1 - centerB;
				vector2::Vec2 centerToCP2 = contactPoint2 - centerB;
				float dotVertB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToVert);
				float dotCPB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToCP);
				float dotCPB2 = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToCP2);
				float eq = dotCPB - dotVertB;
				float eq2 = dotCPB2 - dotVertB;
				if ((m_AlmostEqual(eq,0.f) || m_AlmostEqual(eq2, 0.f) ) && (eq2 < min || eq < min)) {
					min = std::min(eq, eq2);
					if (i == 0) {
						ret.second = 1;
						//break;
					}
					if (i == 1) {
						ret.second = 2;
						//break;
					}
					if (i == 2) {
						ret.second = 4;
						//break;
					}
					if (i == 3) {
						ret.second = 8;
						//break;
					}
				}
			}
			else {
				if (m_AlmostEqualCP(contactPoint1, verticesB[i])) {
					if (i == 0) {
						ret.second = 9;
						break;
					}
					if (i == 1) {
						ret.second = 3;
						break;
					}
					if (i == 2) {
						ret.second = 6;
						break;
					}
					if (i == 3) {
						ret.second = 12;
						break;
					}
				}
			}
		}

		return ret;
	}
}