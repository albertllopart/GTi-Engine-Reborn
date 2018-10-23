#include "ComponentMesh.h"
#include "ImGui/imgui.h"

ComponentMesh::ComponentMesh(): Component(COMPONENT_MESH)
{

}

ComponentMesh::~ComponentMesh()
{
	name = "ComponentMesh";
}

void ComponentMesh::DrawDebug() const
{
	//TODO
}

void ComponentMesh::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Vertices ID: %i", id_vertex);
		ImGui::Text("Num Vertices: %i", num_vertex);
		ImGui::Text("Indexes ID: %i", id_index);
		ImGui::Text("Num Indexes: %i", num_index);
		ImGui::Text("Normals ID: %i", id_normals);
		ImGui::Text("Texture Coords: %i", id_texcoord);
		ImGui::TreePop();
	}
}

float3 ComponentMesh::GetCenter() const
{
	return bbox.Centroid();
}