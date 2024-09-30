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
            std::string prefabName;

            if (prefabData.HasMember("name") && prefabData["name"].IsString()) {
                prefabName = prefabData["name"].GetString();  // Store the name
            }
            
            if (prefabData.HasMember("transform") && prefabData["transform"].IsObject()) {
                //set bitflag
                prefab.m_prefabSignature.set(Ecs::TypeTransformComponent);

                const rapidjson::Value& transform = prefabData["transform"];
                if (transform.HasMember("position") && transform["position"].IsObject()) {
                    prefab.m_transformComponents.position.m_x = transform["position"]["x"].GetFloat();
                    prefab.m_transformComponents.position.m_y = transform["position"]["y"].GetFloat();
                }
                if (transform.HasMember("rotation")) {
                    prefab.m_transformComponents.rotation = transform["rotation"].GetFloat();
                }
                if (transform.HasMember("scale") && transform["scale"].IsObject()) {
                    prefab.m_transformComponents.scale.m_x = transform["scale"]["x"].GetFloat();
                    prefab.m_transformComponents.scale.m_y = transform["scale"]["y"].GetFloat();
                }

            }

            if (prefabData.HasMember("movement") && prefabData["movement"].IsObject()) {

                prefab.m_prefabSignature.set(Ecs::TypeMovemmentComponent);

                const rapidjson::Value& movement = prefabData["movement"];

                if (movement.HasMember("speed")) {
                    prefab.m_movementComponents.Speed = movement["speed"].GetFloat();
                }
                if (movement.HasMember("direction") && movement["direction"].IsObject()) {
                    prefab.m_movementComponents.Direction.m_x = movement["direction"]["x"].GetFloat();
                    prefab.m_movementComponents.Direction.m_y = movement["direction"]["y"].GetFloat();
                }

            }

            if (prefabData.HasMember("collider") && prefabData["collider"].IsObject()) {

                prefab.m_prefabSignature.set(Ecs::TypeColliderComponent);

                 const rapidjson::Value& collider = prefabData["collider"];
                if (collider.HasMember("size") && collider["size"].IsObject()) {
                    prefab.m_colliderComponents.Size.m_x = collider["size"]["x"].GetFloat();
                    prefab.m_colliderComponents.Size.m_y = collider["size"]["y"].GetFloat();
                }
                if (collider.HasMember("offset") && collider["offset"].IsObject()) {
                    prefab.m_colliderComponents.OffSet.m_x = collider["offset"]["x"].GetFloat();
                    prefab.m_colliderComponents.OffSet.m_y = collider["offset"]["y"].GetFloat();
                }
                if (collider.HasMember("layer")) {
                    prefab.m_colliderComponents.Layer = collider["layer"].GetUint();
                }
                if (collider.HasMember("drawDebug")) {
                    prefab.m_colliderComponents.drawDebug = collider["drawDebug"].GetBool();
                }

            }

            if (prefabData.HasMember("player") && prefabData["player"].IsObject()) {

                prefab.m_prefabSignature.set(Ecs::TypePlayerComponent);

                const rapidjson::Value& player = prefabData["player"];
                if (player.HasMember("control")) {
                    prefab.m_playerComponents.Control = player["control"].GetBool();
                }

            }

            // Load RigidBody Component if it exists
            if (prefabData.HasMember("rigidbody") && prefabData["rigidbody"].IsObject()) {

                prefab.m_prefabSignature.set(Ecs::TypeRigidBodyComponent);

                const rapidjson::Value& rigidbody = prefabData["rigidbody"];
                if (rigidbody.HasMember("mass")) {
                    prefab.m_rigidBodyComponents.Mass = rigidbody["mass"].GetFloat();
                }

            }

            // Load Sprite Component if it exists
            if (prefabData.HasMember("sprite") && prefabData["sprite"].IsObject()) {

                prefab.m_prefabSignature.set(Ecs::TypeSpriteComponent);

                const rapidjson::Value& sprite = prefabData["sprite"];
                if (sprite.HasMember("imageID")) {
                    prefab.m_spriteComponents.imageID = sprite["imageID"].GetUint();
                }
                if (sprite.HasMember("frameNumber")) {
                    prefab.m_spriteComponents.frameNumber = sprite["frameNumber"].GetUint();
                }
                
            }




            m_prefabs[prefabName] = prefab;
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

        Ecs::ECS* ecs = Ecs::ECS::GetInstance();

        Ecs::EntityID newEntityID = ecs->CreateEntity();

        //allocate transform component
        //Addon everytime a transform component is added
        
        Ecs::TransformComponent* tc = static_cast<Ecs::TransformComponent*>(ecs->ECS_CombinedComponentPool[Ecs::TypeTransformComponent]->GetEntityComponent(newEntityID));
        *tc = prefab.m_transformComponents;

        //set component back to true
        tc->IsLive = true;
        tc->Entity = newEntityID;

        if (prefab.m_prefabSignature.test(Ecs::TypeMovemmentComponent)) {
            Ecs::MovementComponent* mc = static_cast<Ecs::MovementComponent*>(ecs->AddComponent(Ecs::TypeMovemmentComponent, newEntityID));
            *mc = prefab.m_movementComponents;
            mc->IsLive = true;
            mc->Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(Ecs::TypeColliderComponent)) {
            Ecs::ColliderComponent* cc = static_cast<Ecs::ColliderComponent*>(ecs->AddComponent(Ecs::TypeColliderComponent, newEntityID));
            *cc = prefab.m_colliderComponents;
            cc->IsLive = true;
            cc->Entity = newEntityID;
        }


        if (prefab.m_prefabSignature.test(Ecs::TypePlayerComponent)) {
            Ecs::PlayerComponent* pc = static_cast<Ecs::PlayerComponent*>(ecs->AddComponent(Ecs::TypePlayerComponent, newEntityID));
            *pc = prefab.m_playerComponents;
            pc->IsLive = true;
            pc->Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(Ecs::TypeRigidBodyComponent)) {
            Ecs::RigidBodyComponent* rb = static_cast<Ecs::RigidBodyComponent*>(ecs->AddComponent(Ecs::TypeRigidBodyComponent, newEntityID));
            *rb = prefab.m_rigidBodyComponents;
            rb->IsLive = true;
            rb->Entity = newEntityID;
        }

        if (prefab.m_prefabSignature.test(Ecs::TypeSpriteComponent)) {
            Ecs::SpriteComponent* sc = static_cast<Ecs::SpriteComponent*>(ecs->AddComponent(Ecs::TypeSpriteComponent, newEntityID));
            *sc = prefab.m_spriteComponents;
            sc->IsLive = true;
            sc->Entity = newEntityID;
        }

        LOGGING_INFO("Prefab -> Entity Created Successfully");
        return newEntityID;
    }






}