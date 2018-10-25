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
