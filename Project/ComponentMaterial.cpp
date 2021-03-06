#include "ComponentMaterial.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "E_TextEditor.h"
#include "JSONConfig.h"

ComponentMaterial::ComponentMaterial(): Component(COMPONENT_MATERIAL)
{
	name = "ComponentMaterial";
	material = (ResourceMaterial*)App->resource->CreateResource(RESOURCE_MATERIAL);
	shader = App->renderer3D->shaders_manager->CreateDefaultShaderProgram();
	tex_name = "None";
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
		//ImGui::Text(tex_name.c_str());
		//ImGui::Text("Texture ID: %i", tex_id);
		ImGui::TreePop();
	}
}

void ComponentMaterial::LoadTexture(const char* path)
{
	if (path != nullptr)
	{
		tex_id = App->textures->ImportImage(path);
	}
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
	if (new_name != nullptr)
	{
		tex_name = new_name;
	}
}

void ComponentMaterial::SetTextureSize(uint width, uint height)
{
	tex_width = width;
	tex_height = height;
}

void ComponentMaterial::ShowInspectorWindow()
{
	uint flags = ImGuiTreeNodeFlags_DefaultOpen;
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

		ImGui::Checkbox("Alpha test", &alpha_test);
		
		if (alpha_test)
		{
			ImGui::SliderFloat("alpha value", &alpha_value, 0.0f, 1.0f, "%.2f");
		}

		ImGui::Checkbox("Water Shader", &sample_shader);

		if (sample_shader)
		{
			own_shader = false;
		}

		ImGui::Checkbox("Own Shader", &own_shader);

		if (own_shader)
		{
			sample_shader = false;
			if (ImGui::Button("Edit", ImVec2(100, 0)))
			{
				combo_type = !combo_type;
			}
			if (combo_type)
			{
				const char* items[] = { "VertexShader","FragmentShader" };
				ImGui::Combo("Shader type", &shaders_type, items, IM_ARRAYSIZE(items));
				if (shaders_type != 2)
				{
					ImGui::Text("Shader name:");
					ImGui::SameLine();
					char namedit[50];
					strcpy_s(namedit, 50, "");
					std::string shader_name;
					if (ImGui::InputText("##SHADERname", namedit, 50, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
					{
						shader_name = std::string(namedit).c_str();
					}
					if (shader_name != "")
					{
						switch (shaders_type)
						{
						case 0:
							App->e_windows->GetCodeEditor()->is_vertex_shader = true;
							if (namedit != NULL)
							{
								std::string checker = App->import->shader_importer->Load(namedit, GTI_VERTEX_SHADER);
								if (checker == "null")
								{
									App->e_windows->GetCodeEditor()->SetEditFile(App->import->shader_importer->NewShaderFile(namedit, GTI_VERTEX_SHADER));
								}
								else 
								{
									App->e_windows->GetCodeEditor()->SetEditFile(checker);
								}
							}
							break;
						case 1:
							App->e_windows->GetCodeEditor()->is_vertex_shader = false;
							if (namedit != NULL)
							{
								std::string checker = App->import->shader_importer->Load(namedit, GTI_FRAGMENT_SHADER);
								if (checker == "null")
								{
									App->e_windows->GetCodeEditor()->SetEditFile(App->import->shader_importer->NewShaderFile(namedit, GTI_FRAGMENT_SHADER));
								}
								else
								{
									App->e_windows->GetCodeEditor()->SetEditFile(checker);
								}
							}
							break;
						default:
							return;
						}
						App->e_windows->GetCodeEditor()->code_edit = true;
						App->e_windows->GetCodeEditor()->file_name = shader_name;
					}
					if (ImGui::Button("Open editor"))
					{
						App->e_windows->GetCodeEditor()->code_edit = !App->e_windows->GetCodeEditor()->code_edit;
					}
					ImGui::SameLine();
					if (App->renderer3D->shaders_manager->objects.size() > 1)
					{
						if (ImGui::Button("Create Shader Program"))
						{
							shader = App->renderer3D->shaders_manager->CreateShaderProgram();
						}
					}
				}
			}

		}

		ImGui::TreePop();
	}
}

bool ComponentMaterial::OnSave(JSON_Value* array, uint go_uid)
{
	GenerateUID();
	//create new child
	JSON_Value* comp_value = json_value_init_object();

	//target the new child
	JSON_Object* comp_object = json_value_get_object(comp_value);

	//copy values
	json_object_set_string(comp_object, "Name", name.c_str());
	json_object_set_number(comp_object, "UID", uid);
	json_object_set_number(comp_object, "GameObject", go_uid);
	json_object_set_string(comp_object, "Source", tex_name.c_str());
	json_object_set_number(comp_object, "Width", tex_width);
	json_object_set_number(comp_object, "Height", tex_height);
	json_object_set_number(comp_object, "Type", type);

	//add everything to the components array
	if (array != nullptr)
	{
		JSON_Array* my_array = json_value_get_array(array);
		json_array_append_value(my_array, comp_value);
	}
	


	return true;
}

bool ComponentMaterial::OnLoad(JSONConfig data)
{
	uid = data.GetInt("UID");
	tex_name = data.GetString("Source");
	tex_width = data.GetInt("Width");
	tex_height = data.GetInt("Height");

	if (tex_name != "None")
	{
		tex_id = App->textures->importer->Load(tex_name.c_str());
	}

	return true;
}

bool ComponentMaterial::IsAlphaTest() const
{
	return alpha_test;
}

float ComponentMaterial::GetAlphaValue() const
{
	return alpha_value;
}

ShaderProgram * ComponentMaterial::GetShader() const
{
	return shader;
}
