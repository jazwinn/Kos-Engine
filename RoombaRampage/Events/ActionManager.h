#pragma once
#include <stack>
#include "ModifyAction.h"

namespace actions {
	class ActionManager {
		std::stack<Action*> m_undoStack;
		std::stack<Action*> m_redoStack;

	public:
		void m_doAction(Action* inAction) {
			inAction->m_redoAction();
			m_undoStack.push(inAction);
			while (!m_redoStack.empty()) {
				delete m_redoStack.top();
				m_redoStack.pop();
			}
		}

		void m_undo() {
			if (!m_undoStack.empty()) {
				Action* previousAction = m_undoStack.top();
				previousAction->m_undoAction();
				m_redoStack.push(previousAction);
				m_undoStack.pop();

			}
		}

		void m_redo() {
			if (!m_redoStack.empty()) {
				Action* latestAction = m_redoStack.top();
				latestAction->m_redoAction();
				m_undoStack.push(latestAction);
				m_redoStack.pop();
			}
		}

		void m_push(Action* inAction) {
			m_undoStack.push(inAction);
		}

		~ActionManager() {
			while (!m_undoStack.empty()) {
				delete m_undoStack.top();
				m_undoStack.pop();
			}

			while (!m_redoStack.empty()) {
				delete m_redoStack.top();
				m_redoStack.pop();
			}
		}

		static ActionManager* m_GetManagerInstance();

	private:
		static std::unique_ptr<ActionManager> m_instance;
	};
}