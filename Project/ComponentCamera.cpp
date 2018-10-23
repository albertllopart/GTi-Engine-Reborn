#include "ComponentCamera.h"
#include "Globals.h"
#include "ImGui/imgui.h"

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

	frustum.ProjectionMatrix();

	active = false;
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update()
{

}

void ComponentCamera::SetPos(float3 pos)
{
	frustum.pos = pos;
}

void ComponentCamera::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");
		ImGui::DragFloat("X", &frustum.pos.x, -0.5f, 0.5f);
		ImGui::DragFloat("Y", &frustum.pos.y, -0.5f, 0.5f);
		ImGui::DragFloat("Z", &frustum.pos.z, -0.5f, 0.5f);

		ImGui::DragFloat3("Frustum up", frustum.up.ptr(), -0.5f, 0.5f);
		ImGui::DragFloat3("Frustum front", frustum.front.ptr(), -0.5f, 0.5f);

		ImGui::TreePop();

		ImGui::Text("FOV:");
		if (ImGui::InputFloat("Vertical fov", &fov, 0.5f, 115, 3))
		{
			SetFov();
		}
		ImGui::Checkbox("Active", &active);
	}
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