#ifndef SCENE_H
#define SCENE_H

#include "../Config/pch.h"
#include "../De&Serialization/json_handler.h"

namespace scenes {

	class SceneManager {
	public:
		static SceneManager* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new SceneManager{});
			}
			return m_InstancePtr.get();
		}

		//std::string m_AddScene(std::string filepath);

		// create a json folder
		bool m_CreateNewScene(std::filesystem::path scenepath);

		void m_LoadScene(std::filesystem::path scenepath);

		void m_ReloadScene();

		void m_ClearAllScene();

		void m_ClearScene(std::string scene);

		void m_SaveScene(std::string scene);

		void m_SaveAllActiveScenes();

		void m_SwapScenes(std::string oldscene, std::string newscene , ecs::EntityID id);

		static std::optional<std::string> GetSceneByEntityID(ecs::EntityID entityID);

		std::unordered_map<std::string, std::filesystem::path> m_scenePath;

		std::vector<std::filesystem::path> m_recentFiles;

	private:
		static std::unique_ptr<SceneManager> m_InstancePtr;

	};



}



#endif SCENE_H
