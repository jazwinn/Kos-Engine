#ifndef AUDIOSYS_H
#define AUDIOSYS_H

#include "../ECS.h"
#include "System.h"

namespace ecs {

    class AudioSystem : public ISystem {
    public:
        void m_RegisterSystem(EntityID ID) override;

        void m_DeregisterSystem(EntityID ID) override;

        void m_Init() override;

        void m_Update(const std::string& scene) override;

    private:
        std::vector<AudioComponent*> m_vecAudioComponentPtr;
        std::vector<TransformComponent*> m_vecTransformComponentPtr;

    };

}


#endif // AUDIOSYS_H
