#include "../Config/pch.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "imgui_stdlib.h"
#include "../ECS/ECS.h"
#include "../Asset Manager/AssetManager.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/Layers.h"
#include "../Editor/TilemapCalculations.h"
#include <string>


namespace gui

{
	void ImGuiHandler::m_DrawTilePicker()
	{
		if (ImGui::Button("Back")) 
		{
			m_tilePickerMode = false;
			return;
		}
		ImGui::SameLine();
		ImGui::BeginChild("ChildLb", ImVec2(0, ImGui::GetContentRegionAvail().y));

		ecs::ECS* ecs = ecs::ECS::m_GetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		auto* tmc = static_cast<ecs::TilemapComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETILEMAPCOMPONENT]->m_GetEntityComponent(m_clickedEntityId));

		const float padding = 20.f;
		const float thumbnail = 80.f;
		float cellsize = padding + thumbnail;

		float panelwidth = ImGui::GetContentRegionAvail().x;
		int columns = (int)(panelwidth / cellsize);
		if (columns <= 0) {
			columns = 1;
		}
		ImGui::Columns(columns, 0, false);
		float xWidth = 1.0f / tmc->m_pictureRowLength;
		float yWidth = 1.0f / tmc->m_pictureColumnLength;

		if (assetmanager->m_imageManager.m_imageMap.find(tmc->m_tilemapFile) != assetmanager->m_imageManager.m_imageMap.end())
		{
			for (int i = 0; i < (tmc->m_pictureRowLength * tmc->m_pictureColumnLength); ++i)
			{
				int xIndex = i % tmc->m_pictureRowLength;

				int yIndex = floor(i / tmc->m_pictureRowLength);

				float uvX0 = xIndex * xWidth;               // Left UV coordinate
				float uvX1 = (xIndex + 1) * xWidth;         // Right UV coordinate
				float uvY0 = (yIndex + 1) * yWidth;  // Top UV coordinate (flipped vertically)
				float uvY1 = yIndex * yWidth;        // Bottom UV coordinate

				ImGui::ImageButton(std::to_string(i).c_str(), (ImTextureID)(uintptr_t)assetmanager->m_imageManager.m_imageMap.find(tmc->m_tilemapFile)->second.textureID,
					{ thumbnail ,thumbnail }, { uvX0, uvY1 },{ uvX1, uvY0 }, { 0,0,0,0 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					tmc->m_tileIndex = i;
				}
				ImGui::Text(std::to_string(i).c_str());
				
				ImGui::NextColumn();
				
			}
		}

		
		
		ImGui::EndChild();
	}
}