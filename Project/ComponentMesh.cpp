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
	if (ImGui::TreeNodeEx("ComponentMesh"))
	{
		ImGui::Text("Vertex ID: %i", mesh->id_vertex);
		ImGui::Text("Vertex Ammount: %i", mesh->num_vertex);
		ImGui::Text("Index ID: %i", mesh->id_index);
		ImGui::Text("Index Ammount: %i", mesh->num_index);
		ImGui::Text("Normals ID: %i", mesh->id_normals);
		ImGui::Text("Texture Coords ID: %i", mesh->id_texcoord);
		ImGui::TreePop();
	}
}

float3 ComponentMesh::GetCenter() const
{
	return mesh->bbox.Centroid();
}