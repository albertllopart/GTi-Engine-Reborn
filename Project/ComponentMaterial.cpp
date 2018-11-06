#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "Application.h"

ComponentMaterial::ComponentMaterial(): Component(COMPONENT_MATERIAL)
{
	name = "ComponentMaterial";
}

ComponentMaterial::~ComponentMaterial()
{
	 
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::TreeNodeEx("ComponentMaterial"))
	{
		if (ImGui::BeginPopupContextItem("go_options"))
		{
			if (ImGui::Button("Delete Component"))
			{
				DestroyComponent();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		ImGui::Text(tex_name.c_str());
		ImGui::Text("Texture ID: %i", tex_id);
		ImGui::TreePop();
	}
}

void ComponentMaterial::LoadTexture(const char* path)
{
	tex_id = App->textures->ImportImage(path);
}

uint ComponentMaterial::GetID()const
{
	return tex_id;
}

void ComponentMaterial::SetID(uint id)
{
	tex_id = id;
}

void ComponentMaterial::SetTextureName(const char* new_name)
{
	tex_name = new_name;
}

void ComponentMaterial::SetTextureSize(uint width, uint height)
{
	tex_width = width;
	tex_height = height;
}

void ComponentMaterial::ShowInspectorWindow()
{
	uint flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CheckBox;
	bool node_open = ImGui::TreeNodeEx(name.c_str(), flags);
	if (ImGui::BeginPopupContextItem("go_options"))
	{
		if (ImGui::Button("Delete Component"))
		{
			DestroyComponent();
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (node_open)
	{
		ImGui::Text("Name:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", tex_name.c_str());

		ImGui::Text("Size:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i x %i", tex_width, tex_height);

		ImGui::Text("Image:");
		ImGui::SameLine();
		ImGui::Image((ImTextureID)tex_id, ImVec2(128, 128));

		ImGui::TreePop();
	}
}
