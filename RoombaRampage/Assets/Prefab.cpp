#include "Prefab.h"
#include "../De&Serialization/json_handler.h"
#include "../Debugging/Logging.h"


namespace prefab {

    std::unordered_map<std::string, Prefab> Prefab::m_prefabs;

	void Prefab::m_DeSerializePrefab(std::string path) {

        // Open the JSON file for reading
        std::ifstream inputFile(path);

        if (!inputFile) {
            LOGGING_ERROR("Failed to open Pefab JSON file for reading");
            return;
        }


        std::string fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();

        // Parse the JSON content
        rapidjson::Document doc;
        doc.Parse(fileContent.c_str());

        for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
            const rapidjson::Value& prefabData = doc[i];
            
            Prefab prefab;

            if (prefabData.HasMember("name") && prefabData["name"].IsString()) {
                prefab.m_nameComponents.m_entityName = prefabData["name"].GetString();  // Store the name
                if (prefab.m_nameComponents.m_entityName.empty()) {
                    prefab.m_nameComponents.m_entityName = "OBJECT";
                }
            }
            
            if (prefabData.HasMember("transform") && prefabData["transform"].IsObject()) {
                //set bitflag
                prefab.m_prefabSignature.set(ecs::TYPETRANSFORMCOMPONENT);

                const rapidjson::Value& transform = prefabData["transform"];
                if (transform.HasMember("position") && transform["position"].IsObject()) {
                    prefab.m_transformComponents.m_position.m_x = transform["position"]["x"].GetFloat();
                    prefab.m_transformComponents.m_position.m_y = transform["position"]["y"].GetFloat();
                }
                if (transform.HasMember("rotation")) {
                    prefab.m_transformComponents.m_rotation = transform["rotation"].GetFloat();
                }
                if (transform.HasMember("scale") && transform["scale"].IsObject()) {
                    prefab.m_transformComponents.m_scale.m_x = transform["scale"]["x"].GetFloat();
                    prefab.m_transformComponents.m_scale.m_y = transform["scale"]["y"].GetFloat();
                }

            }

            if (prefabData.HasMember("movement") && prefabData["movement"].IsObject()) {

                prefab.m_prefabSignature.set(ecs::TYPEMOVEMENTCOMPONENT);

                const rapidjson::Value& movement = prefabData["movement"];

                if (movement.HasMember("speed")) {
                    prefab.m_movementComponents.m_Speed = movement["speed"].GetFloat();
                }
                if (movement.HasMember("direction") && movement["direction"].IsObject()) {
                    prefab.m_movementComponents.m_Direction.m_x = movement["direction"]["x"].GetFloat();
                    prefab.m_movementComponents.m_Direction.m_y = movement["direction"]["y"].GetFloat();
                }

            }

            if (prefabData.HasMember("collider") && prefabData["collider"].IsObject()) {

                prefab.m_prefabSignature.set(ecs::TYPECOLLIDERCOMPONENT);

                 const rapidjson::Value& collider = prefabData["collider"];
                if (collider.HasMember("size") && collider["size"].IsObject()) {
                    prefab.m_colliderComponents.m_Size.m_x = collider["size"]["x"].GetFloat();
                    prefab.m_colliderComponents.m_Size.m_y = collider["size"]["y"].GetFloat();
                }
                if (collider.HasMember("offset") && collider["offset"].IsObject()) {
                    prefab.m_colliderComponents.m_OffSet.m_x = collider["offset"]["x"].GetFloat();
                    prefab.m_colliderComponents.m_OffSet.m_y = collider["offset"]["y"].GetFloat();
                }
                if (collider.HasMember("layer")) {
                    prefab.m_colliderComponents.m_Layer = collider["layer"].GetUint();
                }
                if (collider.HasMember("drawDebug")) {
                    prefab.m_colliderComponents.m_drawDebug = collider["drawDebug"].GetBool();
                }

            }

            if (prefabData.HasMember("player") && prefabData["player"].IsObject()) {

                prefab.m_prefabSignature.set(ecs::TYPEPLAYERCOMPONENT);

                const rapidjson::Value& player = prefabData["player"];
                if (player.HasMember("control")) {
                    prefab.m_playerComponents.m_Control = player["control"].GetBool();
                }

            }

            // Load RigidBody Component if it exists
            if (prefabData.HasMember("rigidbody") && prefabData["rigidbody"].IsObject()) {

                prefab.m_prefabSignature.set(ecs::TYPERIGIDBODYCOMPONENT);

                const rapidjson::Value& rigidbody = prefabData["rigidbody"];
                if (rigidbody.HasMember("mass")) {
                    prefab.m_rigidBodyComponents.m_Mass = rigidbody["mass"].GetFloat();
                }

            }

            // Load Sprite Component if it exists
            if (prefabData.HasMember("sprite") && prefabData["sprite"].IsObject()) {

                prefab.m_prefabSignature.set(ecs::TYPESPRITECOMPONENT);

                const rapidjson::Value& sprite = prefabData["sprite"];
                if (sprite.HasMember("imageID")) {
                    prefab.m_spriteComponents.m_imageID = sprite["imageID"].GetUint();
                }
                if (sprite.HasMember("frameNumber")) {
                    prefab.m_spriteComponents.m_frameNumber = sprite["frameNumber"].GetUint();
                }
                
            }




            m_prefabs[prefab.m_nameComponents.m_entityName] = prefab;
        }

        LOGGING_INFO("Load Prefab Json Successful");

	}


    int Prefab::m_CreateEntityFromPrefab(std::string prefabString) {

        //serach for prefab that matches the map

        if (m_prefabs.find(prefabString) == m_prefabs.end()) {
            LOGGING_ERROR("Prefab does not exist");
            return -1;
        }

        Prefab prefab = m_prefabs[prefabString];

        ecs::ECS* ecs = ecs::ECS::m_GetInstance();

        ecs::EntityID newEntityID = ecs->m_CreateEntity();

        //allocate transform component
        //Addon everytime a transform component is added
        {
            ecs::TransformComponent* tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(newEntityID));
            *tc = prefab.m_transformComponents;
            //set component back to true
            tc->m_IsLive = true;
            tc->m_Entity = newEntityID;
        }
        {
            ecs::NameComponent* nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(newEntityID));
            *nc = prefab.m_nameComponents;
            //set component back to true
            nc->m_IsLive = true;
            nc->m_Entity = newEntityID;
        }


        if (prefab.m_prefabSignature.test(ecs::TYPEMOVEMENTCOMPONENT)) {
            ecs::MovementComponent* mc = static_cast<ecs::MovementComponent*>(ecs->m_AddComponent(ecs::TYPEMOVEMENTCOMPONENT, newEntityID));
            *mc = prefab.m_movementComponents;
            mc->m_IsLive = true;
            mc->m_Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(ecs::TYPECOLLIDERCOMPONENT)) {
            ecs::ColliderComponent* cc = static_cast<ecs::ColliderComponent*>(ecs->m_AddComponent(ecs::TYPECOLLIDERCOMPONENT, newEntityID));
            *cc = prefab.m_colliderComponents;
            cc->m_IsLive = true;
            cc->m_Entity = newEntityID;
        }


        if (prefab.m_prefabSignature.test(ecs::TYPEPLAYERCOMPONENT)) {
            ecs::PlayerComponent* pc = static_cast<ecs::PlayerComponent*>(ecs->m_AddComponent(ecs::TYPEPLAYERCOMPONENT, newEntityID));
            *pc = prefab.m_playerComponents;
            pc->m_IsLive = true;
            pc->m_Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(ecs::TYPERIGIDBODYCOMPONENT)) {
            ecs::RigidBodyComponent* rb = static_cast<ecs::RigidBodyComponent*>(ecs->m_AddComponent(ecs::TYPERIGIDBODYCOMPONENT, newEntityID));
            *rb = prefab.m_rigidBodyComponents;
            rb->m_IsLive = true;
            rb->m_Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(ecs::TYPESPRITECOMPONENT)) {
            ecs::SpriteComponent* sc = static_cast<ecs::SpriteComponent*>(ecs->m_AddComponent(ecs::TYPESPRITECOMPONENT, newEntityID));
            *sc = prefab.m_spriteComponents;
            sc->m_IsLive = true;
            sc->m_Entity = newEntityID;
        }

        LOGGING_INFO("Prefab -> Entity Created Successfully");
        return newEntityID;
    }






}