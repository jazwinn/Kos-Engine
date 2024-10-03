/******************************************************************/
/*!
\file      ControlSystem.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     This file contains the declaration of the ControlSystem class,
           which is part of the Entity Component System (ECS) framework.

           The ControlSystem class manages player input and movement control
           for entities in the ECS framework. It processes input for
           entities with PlayerComponent and MovementComponent, updating
           their states based on player commands.

           The class extends the ISystem interface, ensuring it conforms
           to the ECS structure, allowing registration, deregistration,
           initialization, and updating of entities in the system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef CONTROLSYS_H
#define CONTROLSYS_H

#include "System.h"
#include "../ECS/Component/MovementComponent.h"
#include "../ECS/Component/PlayerComponent.h"
#include "../ECS/ECSList.h"

namespace ecs {

    /******************************************************************/
    /*!
    \class   ControlSystem
    \brief   Handles player input and movement for entities in the ECS.

             The ControlSystem class is responsible for processing player
             inputs and updating the corresponding entities' movement and
             behavior. It manages entities that have both a PlayerComponent
             and MovementComponent, allowing them to respond to player commands.
    */
    /******************************************************************/
    class ControlSystem : public ISystem {

    public:

        /******************************************************************/
        /*!
        \fn      void ControlSystem::m_RegisterSystem(EntityID)
        \brief   Registers an entity with the ControlSystem.

                 Adds an entity with PlayerComponent and MovementComponent
                 to the system for input processing and movement control.

        \param   EntityID - The unique identifier of the entity to be registered.
        */
        /******************************************************************/
        void m_RegisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void ControlSystem::m_DeregisterSystem(EntityID)
        \brief   Deregisters an entity from the ControlSystem.

                 Removes an entity from the system, stopping further input
                 and movement control processing for that entity.

        \param   EntityID - The unique identifier of the entity to be deregistered.
        */
        /******************************************************************/
        void m_DeregisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void ControlSystem::m_Init()
        \brief   Initializes the ControlSystem.

                 Sets up necessary structures and prepares the system for
                 processing player inputs and controlling movement for registered entities.
        */
        /******************************************************************/
        void m_Init() override;

        /******************************************************************/
        /*!
        \fn      void ControlSystem::m_Update()
        \brief   Updates the ControlSystem on every frame.

                 Processes player inputs and updates the movement of all
                 registered entities with PlayerComponent and MovementComponent.
        */
        /******************************************************************/
        void m_Update() override;

    private:
        //! Vector storing pointers to PlayerComponent of registered entities
        std::vector<PlayerComponent*> m_vecPlayerComponentPtr;
        //! Vector storing pointers to MovementComponent of registered entities
        std::vector<MovementComponent*> m_vecMovementComponentPtr;

    };

}

#endif // CONTROLSYS_H
