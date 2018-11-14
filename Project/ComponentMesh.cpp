#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"
#include "JSONConfig.h"

ComponentMesh::ComponentMesh(): Component(COMPONENT_MESH)
{
	mesh = new Mesh;
	name = "ComponentMesh";
	source = "None";
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
	mesh = nullptr;
}

void ComponentMesh::Update()
{

}

void ComponentMesh::PostUpdate()
{
	if (my_go->visible)
	{
		App->renderer3D->Draw(this);
	}
}

void ComponentMesh::OnEditor()
{	
}

void ComponentMesh::ShowInspectorWindow()
{
	uint flags = ImGuiTreeNodeFlags_DefaultOpen ;
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
		int num_vertices = (mesh == nullptr) ? 0 : mesh->num_vertex;
		int num_indices = (mesh == nullptr) ? 0 : mesh->num_index;

		ImGui::Text("Source:");
		ImGui::SameLine();
		if (mesh != nullptr)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", source.c_str());

		ImGui::Text("Vertices:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", num_vertices);

		ImGui::Text("Indices:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", num_indices);

		ImGui::TreePop();
	}
}

float3 ComponentMesh::GetCenter() const
{
	return mesh->bbox.Centroid();
}

bool ComponentMesh::OnSave(JSON_Value* array, uint go_uid)
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
	json_object_set_string(comp_object, "Source", source.c_str());
	json_object_set_number(comp_object, "Type", type);

	//add everything to the components array
	if (array != nullptr)
	{
		JSON_Array* my_array = json_value_get_array(array);
		json_array_append_value(my_array, comp_value);
	}

	return true;
}

bool ComponentMesh::OnLoad(JSONConfig data)
{
	uid = data.GetInt("UID");
	source = data.GetString("Source");

	if (source != "None")
	{
		App->import->importer->Load(source.c_str(), this);
	}

	return true;
}

Mesh::Mesh()
{
	//bbox = new AABB();
}
