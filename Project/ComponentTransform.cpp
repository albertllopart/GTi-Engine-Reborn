#include "ComponentTransform.h"
#include "ImGui/imgui.h"
#include "Application.h"
#include "JSONConfig.h"

ComponentTransform::ComponentTransform(float3 pos, float3 scale, Quat rot): Component(COMPONENT_TYPE::COMPONENT_TRANSFORM),pos(pos), new_pos(pos), scale(scale),rot_quat(rot)
{
	name = "ComponentTransform";
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::SetTransform(float3 scale, Quat rotation, float3 position)
{
	this->scale = scale;


	this->rot_quat = rotation;
	rot_euler = rotation.ToEulerXYZ() * RADTODEG;

	this->pos = position;

	UpdateMatrix();
	
}

void ComponentTransform::OnEditor()
{
	//if (ImGui::TreeNodeEx(name.c_str()))
	//{
	//	if (my_go->is_static)
	//	{
	//		ImGui::Text("Position:");
	//		ImGui::Text("X: %.3f", pos.x);
	//		ImGui::SameLine();
	//		ImGui::Text("Y: %.3f", pos.y);
	//		ImGui::SameLine();
	//		ImGui::Text("Z: %.3f", pos.z);
	//		ImGui::Text("Rotation:");
	//		ImGui::Text("X: %.3f", rot_euler.x);
	//		ImGui::SameLine();
	//		ImGui::Text("Y: %.3f", rot_euler.y);
	//		ImGui::SameLine();
	//		ImGui::Text("Z: %.3f", rot_euler.z);
	//		ImGui::Text("Scale:");
	//		ImGui::Text("X: .3f", scale.x);
	//		ImGui::SameLine();
	//		ImGui::Text("Y: .3f", scale.y);
	//		ImGui::SameLine();
	//		ImGui::Text("Z: .3f", scale.z);
	//	}
	//	else
	//	{
	//		ImGui::Text("Position:");
	//		ImGui::DragFloat("X", &pos.x, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Y", &pos.y, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Z", &pos.z, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::Text("Rotation:");
	//		ImGui::DragFloat("X", &rot_euler.x, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Y", &rot_euler.y, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Z", &rot_euler.z, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::Text("Scale:");
	//		ImGui::DragFloat("X", &scale.x, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Y", &scale.y, 0.1f, 0.0f, 0.0f, "%.3f");
	//		ImGui::DragFloat("Z", &scale.z, 0.1f, 0.0f, 0.0f, "%.3f");
	//	}
	//	ImGui::TreePop();
	//}
}

void ComponentTransform::ShowInspectorWindow()
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
		rot_quat.ToEulerXYX();
		float3 temp_rot = rot_euler;

		if (ImGui::DragFloat3("Position##transform_position", &pos.x, 0.3f))
		{
			UpdatePosition(pos);
		}
		if (ImGui::DragFloat3("Rotation##transform_rotation", &temp_rot.x, 0.3f))
		{
			UpdateRotation(temp_rot);
		}
		if (ImGui::DragFloat3("Scale##transform_scale", &scale.x, 0.3f))
		{
			UpdateScale(scale);
		}
		ImGui::TreePop();
	}
	
}

void ComponentTransform::UpdateScale(float3 scale)
{
	this->scale = scale;
	UpdateMatrix();
}

void ComponentTransform::UpdateRotation(float3 rot)
{
	float3 delta = (rot - rot_euler) * DEGTORAD;
	Quat rotation = Quat::FromEulerXYZ(delta.x, delta.y, delta.z);

	rot_quat = rot_quat * rotation;
	rot_euler = rot;
	UpdateMatrix();
}

void ComponentTransform::UpdatePosition(float3 pos)
{
	this->pos = pos;
	UpdateMatrix();
}

void ComponentTransform::UpdateMatrix()
{
	if (!my_go->is_static)
	{
		trans_matrix = float4x4::FromTRS(pos, rot_quat, scale);


		if (my_go->GetParent() != nullptr && my_go->GetParent() != App->editor->GetRoot())
		{
			global_trans_matrix = my_go->GetParent()->GetTransMatrix() * trans_matrix;
		}
		else
		{
			global_trans_matrix = trans_matrix;
		}

		global_trans_matrix_transposed = global_trans_matrix.Transposed();


		//for (uint i = 0; i < my_go->components.size(); i++)
		//{
		//	my_go->components[i]->OnUpdateMatrix(global_trans_matrix);
		//}
		for (uint i = 0; i < my_go->childs.size(); i++)
		{
			my_go->childs[i]->UpdateMatrix();
		}
	}
	my_go->UpdateBBox();
	TransformCamera();
}

void ComponentTransform::TransformCamera()
{
	for (int i = 0; i < my_go->components.size(); ++i)
	{
		if (my_go->components[i]->GetType() == COMPONENT_CAMERA)
		{
			ComponentCamera* c_cam = (ComponentCamera*)my_go->components[i];
			c_cam->OnUpdateMatrix(global_trans_matrix);
		}
	}
}

float4x4 ComponentTransform::GetGlobalMatrix()const
{
	return global_trans_matrix;
}

float4x4 ComponentTransform::GetTransposedGlobalMatrix() const
{
	return global_trans_matrix_transposed;
}

bool ComponentTransform::OnSave(JSON_Value* array, uint go_uid)
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
	
	json_object_set_number(comp_object, "pos.x", pos.x);
	json_object_set_number(comp_object, "pos.y", pos.y);
	json_object_set_number(comp_object, "pos.z", pos.z);

	json_object_set_number(comp_object, "rot.x", rot_euler.x);
	json_object_set_number(comp_object, "rot.y", rot_euler.y);
	json_object_set_number(comp_object, "rot.z", rot_euler.z);

	json_object_set_number(comp_object, "scale.x", scale.x);
	json_object_set_number(comp_object, "scale.y", scale.y);
	json_object_set_number(comp_object, "scale.z", scale.z);

	json_object_set_number(comp_object, "Type", type);

	//add everything to the components array
	JSON_Array* my_array = json_value_get_array(array);
	json_array_append_value(my_array, comp_value);

	return true;
}

bool ComponentTransform::OnLoad(JSONConfig data)
{
	uid = data.GetInt("UID");

	float3 position = float3::zero;
	position.x = data.GetInt("pos.x");
	position.y = data.GetInt("pos.y");
	position.z = data.GetInt("pos.z");

	float3 rotation = float3::zero;
	rotation.x = data.GetInt("rot.x");
	rotation.y = data.GetInt("rot.y");
	rotation.z = data.GetInt("rot.z");

	float3 scale_l = float3::zero;
	scale_l.x = data.GetInt("scale.x");
	scale_l.y = data.GetInt("scale.y");
	scale_l.z = data.GetInt("scale.z");

	UpdatePosition(position);
	UpdateRotation(rotation);
	UpdateScale(scale_l);

	TransformCamera();

	return true;
}