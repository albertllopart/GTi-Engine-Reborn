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
}

E_Inspector::~E_Inspector()
{
	comp_names.clear();
}

bool E_Inspector::Draw()
{
	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_ShowBorders;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowPos(ImVec2(SDL_GetWindowSurface(App->window->window)->w - 350, 20), ImGuiCond_Always);

	ImGui::SetNextWindowSize(ImVec2(350, SDL_GetWindowSurface(App->window->window)->h - 250), ImGuiCond_Always);
	ImGui::Begin("Inspector", NULL, flags);

	ImGui::End();
	return true;
}

void E_Inspector::InspectorComponents()
{




}

void E_Inspector::CleanUp()
{

}
