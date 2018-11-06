#include "ComponentCamera.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/Geometry/LineSegment.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera( float3 pos, float3 front, float3 up, float near_plane_dist, float far_plane_dist, float vertical_fov, float aspect_ratio, FrustumType type):Component( COMPONENT_CAMERA)
{
	name = "Camera";
	frustum.SetPos(pos);
	frustum.SetFront(front);
	frustum.SetUp(up);
	frustum.SetViewPlaneDistances(near_plane_dist, far_plane_dist);
	fov = vertical_fov;
	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspect_ratio));
	this->aspect_ratio = aspect_ratio;
	frustum.type = PerspectiveFrustum;
	frustum.ProjectionMatrix();

	active = false;
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	DrawDebug();
}

void ComponentCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void ComponentCamera::OnEditor()
{
	//if (ImGui::TreeNodeEx(name.c_str()))
	//{
	//	ImGui::Text("Position:");
	//	ImGui::DragFloat("X", &frustum.pos.x, -0.5f, 0.5f);
	//	ImGui::DragFloat("Y", &frustum.pos.y, -0.5f, 0.5f);
	//	ImGui::DragFloat("Z", &frustum.pos.z, -0.5f, 0.5f);

	//	ImGui::DragFloat3("Frustum up", frustum.up.ptr(), -0.5f, 0.5f);
	//	ImGui::DragFloat3("Frustum front", frustum.front.ptr(), -0.5f, 0.5f);



	//	ImGui::Text("FOV:");
	//	if (ImGui::InputFloat("Vertical fov", &fov, 0.5f, 115, 3))
	//	{
	//		SetFov();
	//	}
	//	ImGui::Checkbox("Active", &active);
	//	ImGui::TreePop();
	//}
}



void ComponentCamera::SetAspectRatio(float x, float y)
{
	aspect_ratio = x / y;
	SetFov();
}

void ComponentCamera::SetFov()
{
	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspect_ratio));
}

void ComponentCamera::SetVerticalFOV(float value)
{
	frustum.verticalFov = value;
	frustum.horizontalFov = 2 * Atan(Tan(value * 0.5f) * (aspect_ratio));
}

void ComponentCamera::ShowInspectorWindow()
{
	uint flags = ImGuiTreeNodeFlags_DefaultOpen ; //| ImGuiTreeNodeFlags_CheckBox
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
			ImGui::Checkbox("Enable Culling##show_bb", &culling);

			
			if (ImGui::Checkbox("Set Render Camera##show_bb", &main_camera))
			{
				if (main_camera)
				{
					App->renderer3D->SetCamera(this);
				}
				else
				{
					App->renderer3D->SetCamera(nullptr);
				}
			}
			ImGui::TextWrapped("Aspect ratio:");
			ImGui::SameLine();
			ImGui::Text("%.3f", aspect_ratio);

			ImGui::DragFloat("Near Distance", &frustum.nearPlaneDistance, 0.5, 0.0, frustum.farPlaneDistance);
			
			ImGui::DragFloat("Far Distance", &frustum.farPlaneDistance, 0.5);

			if (ImGui::DragFloat("Field of View", &frustum.verticalFov, 0.01,0.5))
			{
				frustum.horizontalFov = atan(aspect_ratio*tan(frustum.verticalFov / 2)) * 2;
			}

			UpdateMatrix();

			ImGui::TreePop();
		}
}

void ComponentCamera::OnUpdateMatrix(const float4x4 & matrix)
{
	int test;
	if (this != nullptr)
	{
		frustum.SetPos(matrix.TranslatePart());
		frustum.SetFront(matrix.WorldZ().Normalized());
		frustum.SetUp(matrix.WorldY().Normalized());
		UpdateMatrix();
	}
}

void ComponentCamera::UpdateMatrix()
{
	view_matrix = frustum.ViewMatrix();

	view_matrix.Transpose();

	projection_matrix = frustum.ProjectionMatrix();

	projection_matrix.Transpose();

	update_mat = true;
}

void ComponentCamera::DrawDebug() const 
{
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor4f(0.1f, 0.3f, 0.7f, 0.8f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
}

void ComponentCamera::Culling() const
{
	if (culling)
	{
		//culling static objects
		std::vector<GameObject*>items = App->editor->GetStaticGO();
		for (uint i = 0; i < items.size(); i++)
		{
			items[i]->visible = false;
		}

		// Get all static objects that are inside the frustum (accelerated with quadtree)
		App->scene->quadtree.CollectCandidates(candidates_to_cull, frustum);

		// Set visible only these static objects
		while (!candidates_to_cull.empty())
		{
			candidates_to_cull.front()->SetVisible(true); // INSIDE CAMERA VISION
			candidates_to_cull.pop();
		}
	}
}

float * ComponentCamera::GetProjectionMatrix() const
{
	return (float*)projection_matrix.v;
}

Frustum  ComponentCamera::GetFrustum() const
{
	return frustum;
}
