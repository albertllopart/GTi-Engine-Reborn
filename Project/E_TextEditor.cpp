#include "E_TextEditor.h"
#include "ModuleEngineWindows.h"
#include "ImGui/imgui.h"


E_TextEditor::E_TextEditor(ModuleEngineWindows * mom) : E_Windows(mom)
{
}

E_TextEditor::~E_TextEditor()
{
}

bool E_TextEditor::Draw()
{
	if (code_edit)
	{
		ImGui::SetNextWindowPos(ImVec2((SDL_GetWindowSurface(App->window->window)->w/2)-300, (SDL_GetWindowSurface(App->window->window)->h / 2)-300));
		ImGui::SetNextWindowSize(ImVec2(600, SDL_GetWindowSurface(App->window->window)->h - 550), ImGuiCond_Always);
		ImGui::Begin("Shaders Editor");

		ImGui::End();
	}
	

	return true;
}

void E_TextEditor::CleanUp()
{
}

void E_TextEditor::SetEditFile(std::string code)
{
	text_editor.SetText(code);
}
