#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <unordered_map>
#include "../De&Serialization/json_handler.h"
#include <memory>

namespace scenes {

	class SceneManager {
	public:
		static SceneManager* m_GetInstance() {
			if (!m_InstancePtr) {
				m_InstancePtr.reset(new SceneManager{});
			}
			return m_InstancePtr.get();
		}

		void m_AddScene(std::string filepath);

		// create a json folder
		void m_CreateNewScene(std::string scene);

		void m_LoadScene(std::string scene);

		void m_ClearScene(std::string scene);

		void m_SaveScene(std::string scene);

		void m_SaveActiveScene();


		std::string m_jsonFilePath{ "../RoombaRampage/Json/" };

		std::string m_activeScene;
		std::vector<std::string> m_availableScenes;

	private:
		static std::unique_ptr<SceneManager> m_InstancePtr;

	};



}



#endif SCENE_H
