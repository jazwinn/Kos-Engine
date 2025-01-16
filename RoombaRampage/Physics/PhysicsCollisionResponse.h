#pragma once

namespace physicspipe{
	enum CollisionState {
		ENTERED,
		CONTINUOUS,
		EXIT
	};

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