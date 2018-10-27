#include "ComponentCamera.h"
#include "Globals.h"
#include "ImGui/imgui.h"
#include "Glew/include/glew.h"
#include "MathGeoLib/Geometry/LineSegment.h"

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
	DrawDebug();
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



		ImGui::Text("FOV:");
		if (ImGui::InputFloat("Vertical fov", &fov, 0.5f, 115, 3))
		{
			SetFov();
		}
		ImGui::Checkbox("Active", &active);
		ImGui::TreePop();
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

void ComponentCamera::DrawDebug() const //TO TEST IT
{
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();
}
