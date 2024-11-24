#include "../Config/pch.h"
#include "AudioSystem.h"
#include "../Asset Manager/AssetManager.h"
#include "../Asset Manager/Audio.h"

namespace ecs {

    void AudioSystem::m_RegisterSystem(EntityID ID) {
        ECS* ecs = ECS::m_GetInstance();

        if (std::find_if(m_vecAudioComponentPtr.begin(), m_vecAudioComponentPtr.end(),
            [ID](const auto& obj) { return obj->m_Entity == ID; }) == m_vecAudioComponentPtr.end()) {
            m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
            m_vecAudioComponentPtr.push_back((AudioComponent*)ecs->m_ECS_CombinedComponentPool[TYPEAUDIOCOMPONENT]->m_GetEntityComponent(ID));
        }
    }

    void AudioSystem::m_DeregisterSystem(EntityID ID) {
        size_t IndexID = 0;
        for (auto& audioCompPtr : m_vecAudioComponentPtr) {
            if (audioCompPtr->m_Entity == ID) {
                break;
            }
            IndexID++;
        }

        size_t IndexLast = m_vecAudioComponentPtr.size() - 1;
        std::swap(m_vecAudioComponentPtr[IndexID], m_vecAudioComponentPtr[IndexLast]);
        std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

        m_vecAudioComponentPtr.pop_back();
        m_vecTransformComponentPtr.pop_back();
    }

    void AudioSystem::m_Init() {
        m_SystemSignature.set(TYPEAUDIOCOMPONENT);
    }

    void AudioSystem::m_Update(const std::string& scene) {
        ECS* ecs = ECS::m_GetInstance();
        assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();

        if (m_vecAudioComponentPtr.empty()) {
            return;
        }

        int n{ 0 };
        for (auto& audioCompPtr : m_vecAudioComponentPtr) {
            TransformComponent* transform = m_vecTransformComponentPtr[n];
            n++;

            if (transform->m_scene != scene) continue;

            for (auto& audioFile : audioCompPtr->m_AudioFiles) {
                if (audioFile.m_PlayOnStart) {
                    auto it = assetManager->m_audioManager.getSoundMap().find(audioFile.m_Name);
                    if (it != assetManager->m_audioManager.getSoundMap().end()) {
                        auto& sound = it->second;

                        sound->m_SetVolume(audioCompPtr->m_EntityId,audioFile.m_Volume);
                        sound->m_SetLooping(audioCompPtr->m_EntityId,audioFile.m_Loop);

                        if (!sound->m_IsPlaying(audioCompPtr->m_EntityId)) {
                            sound->m_PlaySound(audioCompPtr->m_EntityId);
                            audioFile.m_PlayOnStart = false;
                        }
                    }
                    else {
                        std::cerr << "Audio file " << audioFile.m_Name << " not found in the sound map." << std::endl;
                    }
                }
                // Can add more conditions to handle volume changes, panning, etc. for future reference <<<<<
            }
        }
    }

}
