#include "E_Inspector.h"
#include "ModuleEngineWindows.h"
#include "Application.h"
#include "ImGui/imgui.h"

E_Inspector::E_Inspector(ModuleEngineWindows * mom) : E_Windows(mom)
{
	name = "Inspector";
	/////Components name for inspector
	comp_names[COMPONENT_TRANSFORM] = "ComponentTransform";
	comp_names[COMPONENT_MATERIAL] = "ComponentMaterial";
	comp_names[COMPONENT_MESH] = "ComponentMesh";
	comp_names[COMPONENT_CAMERA] = "ComponentCamera";
}

E_Inspector::~E_Inspector()
{
	comp_names.clear();
}

bool E_Inspector::Draw()
{
	ImGuiWindowFlags flags = 0;
	//flags |= ImGuiWindowFlags_ShowBorders;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(SDL_GetWindowSurface(App->window->window)->w - 350, 20), ImGuiCond_Always);

	ImGui::SetNextWindowSize(ImVec2(350, SDL_GetWindowSurface(App->window->window)->h - 240), ImGuiCond_Always);
	ImGui::Begin("Inspector", NULL, flags);
	ShowComponents();
	ImGui::End();
	
	return true;
}

void E_Inspector::ShowComponents()
{

	GameObject* temp = App->editor->GetSelected();
	if (temp != nullptr)
	{
		temp->ShowInspectorWindow();
		if (temp != App->editor->GetRoot())
		{
			COMPONENT_TYPE select = COMPONENT_NONE;
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(12, 8)); //espai per el nom de les opcions del popup
			if (ImGui::Button("Create Component.."))
			{
				ImGui::OpenPopup("select");
			}
			ImGui::SameLine();
			if (ImGui::BeginPopup("select"))
			{
				for (std::map<COMPONENT_TYPE, const char*>::const_iterator it = comp_names.begin(); it != comp_names.end(); it++)
				{
					if (ImGui::Selectable(it->second))
						select = it->first;
				}

				ImGui::EndPopup();
			}
			if (select != COMPONENT_NONE)
			{
				temp->AddComponent(select);
			}
			ImGui::PopStyleVar();
		}
	}
}

void E_Inspector::CleanUp()
{

}
