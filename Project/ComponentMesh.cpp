#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"

ComponentMesh::ComponentMesh(): Component(COMPONENT_MESH)
{
	mesh = new Mesh;
	name = "ComponentMesh";
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
	App->renderer3D->Draw(this);
}

void ComponentMesh::DrawDebug() const
{
	//TODO
}

void ComponentMesh::OnEditor()
{	
	//if (ImGui::TreeNodeEx("ComponentMesh"))
	//{
	//	ImGui::Text("Vertex ID: %i", mesh->id_vertex);
	//	ImGui::Text("Vertex Ammount: %i", mesh->num_vertex);
	//	ImGui::Text("Index ID: %i", mesh->id_index);
	//	ImGui::Text("Index Ammount: %i", mesh->num_index);
	//	ImGui::Text("Normals ID: %i", mesh->id_normals);
	//	ImGui::Text("Texture Coords ID: %i", mesh->id_texcoord);
	//	ImGui::TreePop();
	//}
}

void ComponentMesh::ShowInspectorWindow()
{
	uint flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_CheckBox;
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

		ImGui::Text("Vertices:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", num_vertices);

		ImGui::Text("Indices:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%i", num_indices);

		ImGui::Text("Mesh Path:");
		ImGui::SameLine();
		if (mesh != nullptr)
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", mesh->name);

		ImGui::TreePop();
	}
}

float3 ComponentMesh::GetCenter() const
{
	return mesh->bbox.Centroid();
}

bool ComponentMesh::OnSave(JSON_Value* value, JSON_Object* node, uint go_uid)
{
	GenerateUID();
	
	//create new child
	std::string add = std::to_string(uid);
	json_object_set_value(node, add.c_str(), json_value_init_object());

	//target the new child
	node = json_object_get_object(node, add.c_str());

	//copy values
	json_object_set_string(node, "Name", name.c_str());
	json_object_set_number(node, "UID", uid);
	json_object_set_number(node, "GameObject", go_uid);

	//target root so a new child can be created
	node = json_value_get_object(value);
	node = json_object_get_object(node, "Scene");

	return true;
}