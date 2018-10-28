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
	if (scene == nullptr)
	{
		scene = App->editor->GetRoot();
	}

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_ShowBorders;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoCollapse;
	flags |= ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);

	ImGui::SetNextWindowSize(ImVec2(350, SDL_GetWindowSurface(App->window->window)->h - 250), ImGuiCond_Always);

	ImGui::Begin("Hierarchy", NULL, flags);

	scene->OnEditor();
	ImGui::BeginChild("EndInspector Zone", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false);

	if (ImGui::BeginPopupContextWindow("GameObject_options"))
	{
		if (ImGui::Button("Create Game Object"))
		{
			GameObject* item = App->editor->CreateEmptyGameObject(nullptr);
			item->SetName("Game Object");
			App->editor->AddToScene(item);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
	ImGui::End();
	return true;
}

void E_Hierarchy::CleanUp()
{
}

