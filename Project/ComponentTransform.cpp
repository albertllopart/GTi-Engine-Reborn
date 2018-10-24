#include "ComponentTransform.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot): Component(COMPONENT_TYPE::COMPONENT_TRANSFORM),pos(pos), new_pos(pos), scale(scale),rot_quat(rot)
{
	name = "ComponentTransform";

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{

}

void ComponentTransform::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		if (my_go->is_static)
		{
			ImGui::Text("Position:");
			ImGui::Text("X: %.3f", pos.x);
			ImGui::SameLine();
			ImGui::Text("Y: %.3f", pos.y);
			ImGui::SameLine();
			ImGui::Text("Z: %.3f", pos.z);
			ImGui::Text("Rotation:");
			ImGui::Text("X: %.3f", rot_euler.x);
			ImGui::SameLine();
			ImGui::Text("Y: %.3f", rot_euler.y);
			ImGui::SameLine();
			ImGui::Text("Z: %.3f", rot_euler.z);
			ImGui::Text("Scale:");
			ImGui::Text("X: .3f", scale.x);
			ImGui::SameLine();
			ImGui::Text("Y: .3f", scale.y);
			ImGui::SameLine();
			ImGui::Text("Z: .3f", scale.z);
		}
		else
		{
			ImGui::Text("Position:");
			ImGui::DragFloat("X",&pos.x,0.1f,0.0f,)







		}
		ImGui::TreePop();
	}
}
