#include "E_Hierarchy.h"
#include "GameObject.h"
#include "ImGui/imgui.h"
#include "Application.h"




E_Hierarchy::E_Hierarchy(ModuleEngineWindows * mom): E_Windows(mom)
{
	name = "Hierarchy";
}

E_Hierarchy::~E_Hierarchy()
{
	scene = nullptr;
}

bool E_Hierarchy::Draw()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_ShowBorders;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);

	ImGui::SetNextWindowSize(ImVec2(350, SDL_GetWindowSurface(App->window->window)->h - 250), ImGuiCond_Always);

	ImGui::Begin("Hierarchy", &active, window_flags);

	if (scene != nullptr)
	{
		scene->OnEditor();
	}
	if (ImGui::BeginPopupContextWindow("go_options"))
	{
		if (ImGui::Button("Generate Game Object"))
		{
			GameObject* item = App->editor->CreateEmptyGameObject(nullptr);
			item->SetName("Game Object");
			App->editor->AddToScene(item);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	return false;
}

void E_Hierarchy::CleanUp()
{
}

void E_Hierarchy::SetSceneGameObject(GameObject * to_set)
{
}
