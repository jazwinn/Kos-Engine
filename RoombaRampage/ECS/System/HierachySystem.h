/******************************************************************/
/*!
\file      HierachySystem.h
\author    Jaz winn
\par       jazwinn.ng@digipen.edu
\date      Sept 29, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef HIERACHYSYS_H
#define HIERACHYSYS_H

#include "System.h"
#include "../ECS/ECSList.h"

namespace ecs {

    class HierachySystem : public ISystem {

    public:

        void m_RegisterSystem(EntityID) override;

        void m_DeregisterSystem(EntityID) override;

        void m_Init() override;

        void m_Update() override;

    private:
        std::vector<TransformComponent*> m_vecTransformComponentPtr;

        std::vector<HierachyComponent*> m_vecHeirachyComponentPtr;

    };

}

#endif HIERACHYSYS_H
