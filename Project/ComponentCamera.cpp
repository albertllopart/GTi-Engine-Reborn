#include "ComponentCamera.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/Geometry/LineSegment.h"
#include "Application.h"
#include "ModuleCamera3D.h"

ComponentCamera::ComponentCamera():Component(COMPONENT_CAMERA)
{
	name = "Camera";

	frustum.type = PerspectiveFrustum;

	frustum.pos = float3::zero;
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 70.0f;

	frustum.verticalFov = DEGTORAD * fov;
	SetAspectRatio(1.3f);

	projection_changed = true;
	
	active = false;
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{
	if (my_go->visible)
	{
		DrawDebug();
	}
}

void ComponentCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void ComponentCamera::OnEditor()
{

}

void ComponentCamera::SetAspectRatio(float ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ratio);
	projection_changed = true;
	aspect_ratio = ratio;
}

float ComponentCamera::GetAspectRatio() const
{
	return aspect_ratio;
}

void ComponentCamera::SetFov()
{
	frustum.verticalFov = DEGTORAD * fov;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspect_ratio));
}

void ComponentCamera::SetVerticalFOV(float value)
{
	frustum.verticalFov = value;
	frustum.horizontalFov = 2 * atanf(tanf(value * 0.5f) * (aspect_ratio));
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
			if (ImGui::Checkbox("Enable Culling##show_bb", &culling))
			{
				App->renderer3D->SetMainCamera(this);
			}

			
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
	if (this != nullptr)
	{
		frustum.pos = matrix.TranslatePart();
		frustum.front = matrix.WorldZ().Normalized();
		frustum.up = matrix.WorldY().Normalized();
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
		std::vector<GameObject*> elements_to_cull;
		App->editor->quadtree.CollectIntersections(elements_to_cull, frustum);

		for (uint i = 0; i < elements_to_cull.size(); i++)
		{
			elements_to_cull[i]->visible = true;
		}
		
	}
}

float * ComponentCamera::GetProjectionMatrix() const
{
	return (float*)projection_matrix.v;
}

float * ComponentCamera::GetViewMatrix() const
{
	return (float*)view_matrix.v;
}

Frustum  ComponentCamera::GetFrustum() const
{
	return frustum;
}

void ComponentCamera::Look(const float3& position)
{
	float3 dir = position - frustum.pos;

	float3x3 matrix = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = matrix.MulDir(frustum.front).Normalized();
	frustum.up = matrix.MulDir(frustum.up).Normalized();
}

float* ComponentCamera::GetOpenGLViewMatrix()
{
	static float4x4 m;

	m = frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.ptr();
}

math::float4x4 ComponentCamera::GetOpenGLProjectionMatrix()
{
	static float4x4 m;

	m = frustum.ProjectionMatrix();
	m.Transpose();

	return m;
}
