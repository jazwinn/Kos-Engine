#pragma once
#include "../Config/pch.h"

namespace DragFloat {
	enum class Member{
		POS,
		ROT,
		SCALE
	};

	enum class Comp {
		TRANSFORM
	};

	class dragFloatCheck {
		bool m_clicked = false;
		Comp m_lastComp;
		Member m_lastMember;
		bool m_calledBefore = false;
		static std::unique_ptr<dragFloatCheck> m_instance;
	public:

		bool m_click(Comp inComp, Member inMemb) {
			if (!m_calledBefore) {
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if(inComp != m_lastComp){
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if (inMemb != m_lastMember) {
				m_calledBefore = true;
				m_lastComp = inComp;
				m_lastMember = inMemb;
				m_clicked = true;
			}
			else if(m_calledBefore){
				return true;
			}
			return false;

		}

		bool m_getCalledBefore() { return m_calledBefore; }
		Member m_getPrevMem() { return m_lastMember; }

		static dragFloatCheck* m_GetInstance();
	};
}