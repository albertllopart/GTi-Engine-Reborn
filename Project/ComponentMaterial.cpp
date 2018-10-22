#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "Application.h"

ComponentMaterial::ComponentMaterial(GameObject * my_go): Component(my_go, COMPONENT_TYPE::COMPONENT_MATERIAL)
{
	name = "ComponentMaterial";
}

ComponentMaterial::~ComponentMaterial()
{
	 
}

void ComponentMaterial::OnEditor()
{
	if (ImGui::TreeNodeEx("Material"))
	{
		ImGui::Text(name.c_str());
		ImGui::Text("Texture ID: %i", tex_id);
		ImGui::TreePop();
	}
}

void ComponentMaterial::LoadTexture(const char* path)
{
	tex_id = App->textures->ImportImage(path);
}
