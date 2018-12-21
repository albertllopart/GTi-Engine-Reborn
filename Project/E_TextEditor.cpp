#include "E_TextEditor.h"
#include "ModuleEngineWindows.h"
#include "ImGui/imgui.h"


E_TextEditor::E_TextEditor(ModuleEngineWindows * mom) : E_Windows(mom)
{
	text_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
}

E_TextEditor::~E_TextEditor()
{
}

bool E_TextEditor::Draw()
{
	if (code_edit)
	{
		auto cpos = text_editor.GetCursorPosition();
		ImGui::Begin("Shader editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
		ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					auto textToSave = text_editor.GetText();
				}
				if (ImGui::MenuItem("Quit", "Alt-F4"))
					code_edit = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = text_editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					text_editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && text_editor.CanUndo()))
					text_editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && text_editor.CanRedo()))
					text_editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, text_editor.HasSelection()))
					text_editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && text_editor.HasSelection()))
					text_editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && text_editor.HasSelection()))
					text_editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					text_editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					text_editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(text_editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Dark palette"))
					text_editor.SetPalette(TextEditor::GetDarkPalette());
				if (ImGui::MenuItem("Light palette"))
					text_editor.SetPalette(TextEditor::GetLightPalette());
				if (ImGui::MenuItem("Retro blue palette"))
					text_editor.SetPalette(TextEditor::GetRetroBluePalette());
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("Save & Compile"))
		{
			ShaderObject* temp;
			if (is_vertex_shader)
			{
				App->import->shader_importer->SaveShaderFile(file_name.c_str(), text_editor.GetText(), GTI_VERTEX_SHADER);
				temp = new ShaderObject(shader_type::GTI_VERTEX_SHADER);
				temp->data = (GLchar*)text_editor.GetText().c_str();
			}
			else
			{
				App->import->shader_importer->SaveShaderFile(file_name.c_str(), text_editor.GetText(), GTI_FRAGMENT_SHADER);
				temp = new ShaderObject(shader_type::GTI_FRAGMENT_SHADER);
				temp->data = (GLchar*)text_editor.GetText().c_str();
			}
			bool ret = App->renderer3D->shaders_manager->LoadShaderObject(temp);
			if (!ret)
			{
				App->imgui->AddConsoleLog("SHADER CREATER WITH EDITOR NOT LOADED OR COMPILED CORRECTLY!!!");
			}
		}

		text_editor.Render("Editor");
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
