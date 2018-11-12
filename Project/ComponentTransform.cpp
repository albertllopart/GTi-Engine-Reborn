#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ImGui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
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

void ComponentTransform::Update()
{
	if (my_go == App->editor->GetSelected())
	{
		ShowGizmo(*App->camera->GetCamera());
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

float3 ComponentTransform::GetPosition() const
{
	return pos;
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
	if (array != nullptr)
	{
		JSON_Array* my_array = json_value_get_array(array);
		json_array_append_value(my_array, comp_value);
	}

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

void ComponentTransform::SetPosition(float3 pos)
{
	this->pos = pos;
}

void ComponentTransform::ShowGizmo(ComponentCamera & camera)
{
	ImGuizmo::Enable(true);
	if (my_go->is_static)
	{
		ImGuizmo::Enable(false);
	}

	static ImGuizmo::OPERATION currentOperation(ImGuizmo::TRANSLATE);
	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::TRANSLATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::ROTATE;
	}
	else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
	{
		currentOperation = ImGuizmo::SCALE;
	}

	float* projMatrixg = camera.GetFrustum().ViewProjMatrix().Transposed().ptr();
	float* transMatrix = global_trans_matrix.Transposed().ptr();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGuizmo::Manipulate(float4x4::identity.ptr(), projMatrixg, currentOperation, ImGuizmo::WORLD, transMatrix);
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && App->input->GetKey(SDL_SCANCODE_RALT) == KEY_IDLE && !ImGui::GetIO().WantCaptureKeyboard)
	{
		if (ImGuizmo::IsUsing())
		{
			ImGuizmo::DecomposeMatrixToComponents(transMatrix, (float*)pos.ptr(), (float*)rot_euler.ptr(), (float*)scale.ptr());
			global_trans_matrix.Transpose();
			ImGuizmo::RecomposeMatrixFromComponents((float*)pos.ptr(), (float*)rot_euler.ptr(), (float*)scale.ptr(), global_trans_matrix.ptr());
			global_trans_matrix.Transpose();
		}
	}
	UpdateMatrix();
}