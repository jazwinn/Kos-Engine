
#include "../Config/pch.h"
#include "ScriptVariable.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/class.h>
#include <mono/metadata/metadata.h>

#include "../Asset Manager/AssetManager.h"

#include "imgui.h"

namespace scripteditor {

	void ScriptEditor::DisplayScriptComponents(std::string script, ecs::EntityID entityID)
	{
		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		auto* sc = static_cast<ecs::ScriptComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPESCRIPTCOMPONENT]->m_GetEntityComponent(entityID));

        if (sc->m_scriptInstances.find(script) == sc->m_scriptInstances.end()) return;
        if (assetmanager->m_scriptManager.m_ScriptMap.find(script) == assetmanager->m_scriptManager.m_ScriptMap.end()) return;

		MonoImage* image = assetmanager->m_scriptManager.m_ScriptMap.find(script)->second.m_image;
		MonoClass* scriptclass = mono_class_from_name(image, "Namespace", script.c_str());

        void* iter = nullptr;
        MonoClassField* field;
        while ((field = mono_class_get_fields(scriptclass, &iter)) != nullptr) {
            // Check if the field is public
            if ((mono_field_get_flags(field) & 0x0006) == 0x0006) { //0x0006 representing public
                const char* fieldName = mono_field_get_name(field);
                MonoType* fieldType = mono_field_get_type(field);
                int fieldTypeCode = mono_type_get_type(fieldType);

                MonoClassField* field = mono_class_get_field_from_name(scriptclass, fieldName);

                //std::cout << "Found public field: " << fieldName << " (Type Code: " << fieldTypeCode << ")\n";

                // Change the value based on type (example for int and float)
                if (fieldTypeCode == MONO_TYPE_I4) { // Type code for int
                    int integer;
                    mono_field_get_value(sc->m_scriptInstances.find(script)->second, field, &integer);

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::DragInt(fieldName, &integer, 0.1f, -1000.0f, 1000.f, "%.2f")) {
                        mono_field_set_value(sc->m_scriptInstances.find(script)->second, field, &integer);
                    }
                }
                else if (fieldTypeCode == MONO_TYPE_R4) { // Type code for float
                    float _float;
                    mono_field_get_value(sc->m_scriptInstances.find(script)->second, field, &_float);

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::DragFloat(fieldName, &_float, 0.1f, -1000.0f, 1000.f, "%.2f")) {
                        mono_field_set_value(sc->m_scriptInstances.find(script)->second, field, &_float);
                    }


                }
                else if (fieldTypeCode == MONO_TYPE_BOOLEAN) { // Type code for float
                    bool _bool;
                    mono_field_get_value(sc->m_scriptInstances.find(script)->second, field, &_bool);

                    ImGui::AlignTextToFramePadding();
                    if (ImGui::Checkbox(fieldName, &_bool)) {
                        mono_field_set_value(sc->m_scriptInstances.find(script)->second, field, &_bool);
                    }


                }
                else if (fieldTypeCode == MONO_TYPE_STRING) { // Type code for string


                }
            }
        }

	}




}


