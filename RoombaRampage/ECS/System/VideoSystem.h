#ifndef VIDEOSYS_H
#define VIDEOSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include "../ECS/Component/VideoComponent.h"
#include "../ECS/Component/NameComponent.h"
#include "../ECS/Component/TransformComponent.h"

namespace ecs {

    class VideoSystem : public ISystem {

    public:

        /******************************************************************/
        /*!
        \fn      void TransformSystem::m_RegisterSystem(EntityID)
        \brief   Registers an entity with the transform system.
        \param   EntityID - The ID of the entity to be registered.
        \details Adds the entity to the transform system, allowing it to manage transformations for entities.
        */
        /******************************************************************/
        void m_RegisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void TransformSystem::m_DeregisterSystem(EntityID)
        \brief   Deregisters an entity from the transform system.
        \param   EntityID - The ID of the entity to be deregistered.
        \details Removes the entity from the transform system, stopping its transformation updates.
        */
        /******************************************************************/
        void m_DeregisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void TransformSystem::m_Init()
        \brief   Initializes the transform system.
        \details Prepares necessary resources and sets up the transform system for use.
        */
        /******************************************************************/
        void m_Init() override;

        /******************************************************************/
        /*!
        \fn      void TransformSystem::m_Update(const std::string& updateContext)
        \brief   Updates the transform system.
        \param   updateContext - The context or description of the update.
        \details Performs updates on all entities registered in the system for transformation management.
        */
        /******************************************************************/
        void m_Update(const std::string&) override;


    private:

        /******************************************************************/
        /*!
        \var     std::vector<TransformComponent*> m_vecTransformComponentPtr
        \brief   Stores pointers to TransformComponent instances for registered entities.
        \details Used to efficiently access and update the transform data for entities in the system.
        */
        /******************************************************************/
        std::vector<TransformComponent*> m_vecTransformComponentPtr;

        std::vector<VideoComponent*> m_vecVideoComponentPtr;

        std::vector<NameComponent*> m_vecNameComponentPtr;
    };

}


#endif VIDEOSYS_H
