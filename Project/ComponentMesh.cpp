#include "Application.h"
#include "ComponentMesh.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"

ComponentMesh::ComponentMesh(): Component(COMPONENT_MESH)
{

}

ComponentMesh::~ComponentMesh()
{
	name = "ComponentMesh";
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
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Vertex ID: %i", id_vertex);
		ImGui::Text("Vertex Ammount: %i", num_vertex);
		ImGui::Text("Index ID: %i", id_index);
		ImGui::Text("Index Ammount: %i", num_index);
		ImGui::Text("Normals ID: %i", id_normals);
		ImGui::Text("Texture Coords ID: %i", id_texcoord);
		ImGui::TreePop();
	}
}

float3 ComponentMesh::GetCenter() const
{
	return bbox.Centroid();
}