
/******************************************************************/
/*!
\file      LogicSystem.h
\author    
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef LOGICSYS_H
#define LOGICSYS_H

#include "System.h"
#include "../ECS/Component/ScriptComponent.h"
#include "../ECS/ECSList.h"

namespace ecs {


    class LogicSystem : public ISystem {

    public:


        void m_RegisterSystem(EntityID) override;


        void m_DeregisterSystem(EntityID) override;


        void m_Init() override;

        void m_StartLogic();

        void m_Update() override;

    private:
        //! Vector storing pointers to PlayerComponent of registered entities
        std::vector<ScriptComponent*> m_vecScriptComponentPtr;


    };

}

#endif LOGICSYS_H
